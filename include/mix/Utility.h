#pragma once

#include <algorithm>
#include <cmath>
#include <loki/Typelist.h>

namespace Mix {
  
  template<typename T>
  T Clamp(T x, T low, T high) {
    assert(low <= high);
    return std::min(std::max(x, low), high);
  }
  float Wrap(float x, float low, float high) {
    assert(low < high);
    const float n = std::fmod(x - low, high - low);
    return (n >= 0) ? (n + low) : (n + high);
  }
  
  template<typename T>
  class Range {
  public:
    Range(T min, T max) : min_(min), max_(max) {
      assert(min_ <= max_);
    }
    bool IsInside(T value) const {
      return (min_ <= value) && (value <= max_);
    }
    T Clamp(T value) const {
      return Mix::Clamp(value, min_, max_);
    }
    T Wrap(T value) const {
      return Mix::Wrap(value, min_, max_);
    }
    T Min() const {
      return min_;
    }
    T Max() const {
      return max_;
    }
  private:
    T min_;
    T max_;
  };
  
  class Timer {
  public:
    Timer(float limitTime)
      : time_(0), limitTime_(limitTime) {
    }
    void Update(float deltaTime) {
      time_ = std::min(time_ + deltaTime, limitTime_);
    }
    void Reset() {
      time_ = 0;
    }
    bool IsTimeout() const {
      return time_ >= limitTime_;
    }
  private:
    float time_;
    const float limitTime_;
  };
  
  class PresentTimer {
  public:
    PresentTimer(float limitTime)
      : timer_(limitTime), lastTime_(0) {
    }
    void Update(float nowTime) {
      float deltaTime = nowTime - lastTime_;
      timer_.Update(deltaTime);
      lastTime_ = nowTime;
    }
    void Reset() {
      timer_.Reset();
    }
    bool IsTimeout() const {
      return timer_.IsTimeout();
    }
  private:
    Timer timer_;
    float lastTime_;
  };
  template<class Timer, class Action>
  class Invoker
  {
  public:
    Invoker(float limitTime, const Action& action)
      : timer_(limitTime), action_(action) {
    }
    void operator()(float time) {
      Update(time);
      if (IsTimeout()) {
        action_();
        Reset();
      }
    }
  private:
    void Update(float time) {
      timer_.Update(time);
    }
    bool IsTimeout() {
      return timer_.IsTimeout();
    }
    void Reset() {
      timer_.Reset();
    }
  private:
    Timer timer_;
    Action action_;
  };

  template<class T>
  class Less {
  public:
    Less(const T& value)
      : value_(value) {
    }
    bool operator < (const Less& rhs) const {
      return value_ < rhs.value_;
    }
  private:
    T value_;
  };

  template<class TList>
  struct LeafLength;

  template<>
  struct LeafLength<Loki::NullType>
  {
    enum { value = 0 };
  };
  template<class T, class U>
  struct LeafLength<Loki::Typelist<T, U>>
  {
    enum { value = 1 + LeafLength<U>::value };
  };
  template<class T1, class T2, class U>
  struct LeafLength<Loki::Typelist<Loki::Typelist<T1, T2>, U>>
  {
    enum { value = LeafLength<Loki::Typelist<T1, T2>>::value + LeafLength<U>::value };
  };

  template<class TList, class T>
  struct LeafIndexOf;

  template<class T>
  struct LeafIndexOf<Loki::NullType, T>
  {
    enum { value = -1 };
  };
  template<class Tail, class T>
  struct LeafIndexOf<Loki::Typelist<T, Tail>, T>
  {
    enum { value = 0 };
  };
  template<class Head, class Tail, class T>
  struct LeafIndexOf<Loki::Typelist<Head, Tail>, T>
  {
    enum { value = Loki::TL::IndexOf<Loki::Typelist<Head, Tail>, T>::value };
  };
  template<class Head1, class Head2, class Tail, class T>
  struct LeafIndexOf<Loki::Typelist<Loki::Typelist<Head1, Head2>, Tail>, T>
  {
  private:
    using Head = Loki::Typelist<Head1, Head2>;
    enum { temp = LeafIndexOf<Tail, T>::value };
  public:
    enum { value = temp == -1 ?
	   LeafIndexOf<Head, T>::value : LeafLength<Head>::value + temp };
  };

  template<class TList>
  struct MaxSizeOf
  {
    enum { value = 0 };
  };
      
  template<>
  struct MaxSizeOf<Loki::NullType>
  {
    enum { value = -1 };
  };
  template<class T>
  struct MaxSizeOf<Loki::Typelist<T, Loki::NullType>>
  {
    enum { value = sizeof(T) };
  };
  template<class T1, class T2>
  struct MaxSizeOf<Loki::Typelist<T1, T2>>
  {
    enum {
      value =
      (std::max)(
        static_cast<std::size_t>(sizeof(T1)),
        static_cast<std::size_t>(MaxSizeOf<T2>::value)
      )
    };
  };
      
}

