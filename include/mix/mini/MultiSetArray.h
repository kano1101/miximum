//
//  Created by 狩野 亮 on 2020/03/29.
//  Copyright © 2020年 狩野 亮. All rights reserved.
//

#pragma once

#include <algorithm>

#include <mix/mini/DualityList.h>
#include <mix/mini/PerformanceTester.h>

namespace Mix
{
  
  template
  <
    typename Key,
    std::size_t MaxSize,
    class Compare = std::less<Key>,
    class Container = DualityList<Key, MaxSize>
    >
  class MultiSetArray
  {
  public:
    using key_type               = Key;
    using value_type             = Key;
    using key_compare            = Compare;
    using value_compare          = Compare;
      
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
      
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;

    using container_type         = Container;
    using iterator               = typename container_type::iterator;
      
  private:
    container_type c_;
      
  public:
    template<class ...Args>
    MultiSetArray(size_type sz, Args ...args)
      : c_(sz, args...) {
    }
    
  public:
    iterator begin() noexcept { return c_.begin(); }
    iterator end() noexcept { return c_.end(); }
      
  public:
    bool empty() const noexcept { return c_.empty(); }
    bool full() const noexcept { return c_.full(); }
    std::size_t max_size() const noexcept { return c_.max_size(); }
    std::size_t size() const noexcept { return c_.size(); }
      
  private:
    iterator lower_bound(const key_type& x) const noexcept {
      Performance::Instance().Start(5000);
      auto b = c_.begin();
      Performance::Instance().Finish(5000);
      Performance::Instance().Start(6000);
      auto e = c_.end();
      Performance::Instance().Finish(6000);
      Performance::Instance().Start(7000);
      auto r = std::lower_bound(b, e, x, key_compare());
      Performance::Instance().Finish(7000);
      return r;
    }
      
  public:
    iterator insert(const key_type& x) noexcept {
      Performance::Instance().Start(10000);
      auto it = lower_bound(x);
      Performance::Instance().Finish(10000);
      Performance::Instance().Start(20000);
      auto ret = c_.insert(it, x);
      Performance::Instance().Finish(20000);
      return ret;
    }
    iterator erase(iterator target) noexcept { return c_.erase(target); }

  // public:
  //   static iterator GetIterator(pointer p) noexcept {
  //     return container_type::GetIterator(p);
  //   }
      
  }; // class MultiSetArray
    
} // namespace Mix

