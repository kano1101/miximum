#pragma once

#include <iostream>
#include <iomanip>
#include <time.h>
#include <loki/AssocVector.h>
#include <loki/Singleton.h>

namespace Mix {

  template<typename IdentifierType>
  class PerformanceTester {
  public:
    PerformanceTester() {}
    void Start(IdentifierType id) {
      double now = static_cast<double>(clock()) / static_cast<double>(CLOCKS_PER_SEC);
      before_[id] = now;
    }
    void Finish(IdentifierType id) {
      double now = static_cast<double>(clock()) / static_cast<double>(CLOCKS_PER_SEC);
      result_[id] += now - before_[id];
    }
    void Print() {
      std::cout << "Print start." << std::endl;
      std::for_each(
        result_.begin(), result_.end(),
        [&] (auto& e) {
          std::cout << "ID:" << e.first << "->" << e.second << "(秒)" << std::endl;
        });
      std::cout << "Print finish." << std::endl;
    }
  private:
    const clock_t tb = CLOCKS_PER_SEC;
    Loki::AssocVector<IdentifierType, double> before_;
    Loki::AssocVector<IdentifierType, double> result_;
  };

}

using Performance = Loki::SingletonHolder<Mix::PerformanceTester<int>>;
