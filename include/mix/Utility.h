#include <algorithm>
#include <cmath>

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
  class Executor
  {
  public:
    Executor(float limitTime, const Action& action)
      : timer_(limitTime), action_(action) {
    }
    void ExecuteOnTime(float time) {
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

}
