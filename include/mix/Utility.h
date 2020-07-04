#pragma once

#include <algorithm>
#include <cmath>
#include <loki/Typelist.h>
#include <loki/TypeTraits.h>
#pragma GCC diagnostic ignored "-Wunused-function"

#define MIX_LOKI_DEFINE_SINGLETON_CLASS(SingletonType, FriendType) \
    private: \
    SingletonType() {} \
    SingletonType(const SingletonType&) {} \
    SingletonType& operator=(const SingletonType&) { return *this; } \
    SingletonType(SingletonType&&) {} \
    SingletonType& operator=(SingletonType&&) { return *this; } \
    friend class FriendType<SingletonType>;

#define MIX_LOKI_DEFINE_SINGLETON_STRUCT(SingletonType, FriendType) \
    private: \
    SingletonType() {} \
    SingletonType(const SingletonType&) {} \
    SingletonType& operator=(const SingletonType&) { return *this; } \
    SingletonType(SingletonType&&) {} \
    SingletonType& operator=(SingletonType&&) { return *this; } \
    friend struct FriendType<SingletonType>;


namespace Mix {

  namespace {
    template<typename T>
    T Clamp(T x, T low, T high) {
      assert(low <= high);
      return std::min(std::max(x, low), high);
    }
    template<typename T>
    T Wrap(T x, T low, T high) {
      assert(low < high);
      const T n = (x - low) % (high - low);
      return (n >= 0) ? (n + low) : (n + high);
    }
    template<>
    float Wrap<float>(float x, float low, float high) {
      assert(low < high);
      const float n = std::fmod(x - low, high - low);
      return (n >= 0) ? (n + low) : (n + high);
    }
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

  namespace Private {
    template<typename T>
    class inner_less {
    public:
      bool operator()(const T& lhs, const T& rhs) {
        return lhs < rhs;
      }
    };
    template<typename T>
    class inner_less<T*> {
    public:
      bool operator()(const T* const lhs, const T* const rhs) {
        return (lhs != nullptr) && (rhs != nullptr) ? *lhs < *rhs : assert(!"inner_lessでのNullポインタ参照エラー");
      }
    };
  }

  //////////////////////////////////////////////////////////////////////
  // 最良値を保持させるために使うクラス
  template<typename T, class Predicate = Private::inner_less<T>>
  class Best {
  public:
    Best() : most_({}) {
    }
    Best(const T& sample) : most_(sample) {
    }
    T RecordIfBetter(const T& sample, Predicate pred = Predicate()) {
      return pred(most_, sample) ? most_ = sample : most_;
    }
  private:
    T most_;
  };
  template<typename T>
  class Best<T*, typename Best<T*>::Predicate> {
  public:
    Best(const T* const sample = nullptr) : most_(sample) {
    }
    template<class Predicate = typename Best<T>::Compare>
    T* RecordIfBetter(const T* const sample, Predicate pred = Predicate()) {
      if ( sample == nullptr ) return most_;
      if ( !IsExists() || pred(most_, sample) ) return most_ = sample;
      return most_;
    }
    bool IsExists() const {
      return most_ != nullptr;
    }
  private:
    const T* most_;
  };

  ///////////////////////////////////////////////////////////////////////
  // 定期実行に使えるTimerクラス
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
    const T& value_;
  };

  namespace TL {

    template
      <
      typename T1 = Loki::NullType, typename T2 = Loki::NullType, typename T3 = Loki::NullType,
      typename T4 = Loki::NullType, typename T5 = Loki::NullType, typename T6 = Loki::NullType,
      typename T7 = Loki::NullType, typename T8 = Loki::NullType, typename T9 = Loki::NullType,
      typename T10 = Loki::NullType, typename T11 = Loki::NullType, typename T12 = Loki::NullType,
      typename T13 = Loki::NullType, typename T14 = Loki::NullType, typename T15 = Loki::NullType,
      typename T16 = Loki::NullType, typename T17 = Loki::NullType, typename T18 = Loki::NullType
      >
    struct Connect {
    private:
      typedef typename Connect
        <
        T2, T3, T4,
        T5, T6, T7,
        T8, T9, T10,
        T11, T12, T13,
        T14, T15, T16,
        T17, T18
        >
        ::Result TailResult;

    public:
      typedef typename Loki::TL::Append<T1, TailResult>::Result Result;
    };

    template<typename T>
    struct Connect<T, Loki::NullType> {
      typedef T Result;
    };

    template<>
    struct Connect<> {
      typedef Loki::NullType Result;
    };

    ///////////////////////////////////////////////////////////////////////
    // タイプリストのネストされたリスト版Length
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

    /////////////////////////////////////////////////////////////////////
    // タイプリストのネストされたリスト版IndexOf
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

  } // namespace TL
  
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

  template<class T> struct Convert2Raw                      { using Result = T; };
  template<class T> struct Convert2Raw<T*>                  { using Result = T; };
  template<class T> struct Convert2Raw<const T*>            { using Result = T; };
  template<class T> struct Convert2Raw<T* const>            { using Result = T; };
  template<class T> struct Convert2Raw<const T* const>      { using Result = T; };
  template<class T> struct Convert2Raw<T&>                  { using Result = T; };
  template<class T> struct Convert2Raw<const T&>            { using Result = T; };

  template<class T> struct Convert2ConstRef                 { using Result = const T&; };
  template<class T> struct Convert2ConstRef<T*>             { using Result = const T&; };
  template<class T> struct Convert2ConstRef<const T*>       { using Result = const T&; };
  template<class T> struct Convert2ConstRef<T* const>       { using Result = const T&; };
  template<class T> struct Convert2ConstRef<const T* const> { using Result = const T&; };
  template<class T> struct Convert2ConstRef<T&>             { using Result = const T&; };
  template<class T> struct Convert2ConstRef<const T&>       { using Result = const T&; };
  
  template<class T> struct Convert2Lightweight
  { using Result = typename Loki::TypeTraits<T>::ParameterType; };
}
