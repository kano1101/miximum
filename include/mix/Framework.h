#pragma once

#include <mix/Utility.h>

namespace Mix {

  class Framework {
  public:
    class MainWindowInterface {
    public:
      virtual ~MainWindowInterface() {}
      virtual bool IsContinue() const = 0;
      virtual void CleanUp() const = 0;
      virtual float Now() const = 0;
    };

  public:
    Framework(MainWindowInterface& main, std::function<void (void)> action, double fps = 59.94)
      : main_(main)
      , invoker_(1.0 / fps, action) {
    }
    void Mainloop() {
      while(main_.IsContinue()) {
        invoker_(main_.Now());
        main_.CleanUp();
      }
    }
    
  private:
    MainWindowInterface& main_;
    Mix::Invoker<Mix::PresentTimer, std::function<void (void)>> invoker_;
  };

} // namespace Mix

