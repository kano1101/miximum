//
//  Created by 狩野 亮 on 2020/03/29.
//  Copyright © 2020年 狩野 亮. All rights reserved.
//

#ifndef _MIX_MULTI_MAP_ARRAY_H_
#define _MIX_MULTI_MAP_ARRAY_H_

#include <loki/Typelist.h>
#include <loki/Singleton.h>

#include "MultiSetArray.h"

namespace Mix
{
  
  struct less_inner {
    template < class T, class U >
    bool operator () ( const T &lhs, const U &rhs) const
    { return bool(lhs < rhs); }
  };

  template
  <
    typename Key,
    typename T,
    std::size_t WorkCount,
    class Compare = less_inner,
    class Container = MultiSetArray<std::pair<const Key, T>, WorkCount, Compare>
  >
  class MultiMapArray
  {
  public:
    using key_type               = Key;
    using value_type             = T;
    using pair_type              = std::pair<const Key, T>;

    using key_compare            = Compare;
      
    using reference              = pair_type&;
    using const_reference        = const pair_type&;
    using pointer                = pair_type*;
    using const_pointer          = const pair_type*;
      
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
      
    using container_type         = Container;
    using iterator               = typename container_type::iterator;
      
  private:
    container_type c_;
      
  public:
    iterator insert(const pair_type& x) noexcept
    {
      return c_.insert(x);
    }
    iterator erase(iterator target) noexcept
    {
      return c_.erase(target);
    }
      
  public:
    iterator begin() noexcept
    {
      return c_.begin();
    }
    iterator end() noexcept
    {
      return c_.end();
    }

  public:
    static iterator ConvertToPairIterator(void* head) noexcept
    {
      static pair_type p;
      std::uintptr_t offset =
        reinterpret_cast<std::uintptr_t>(&(p.second)) -
        reinterpret_cast<std::uintptr_t>(&p);
      return container_type::Convert(
        reinterpret_cast<pointer>(
          reinterpret_cast<void*>(
            reinterpret_cast<std::uintptr_t>(head) -
            offset
          )
        )
      );
    }
    
  }; // class SortedStorage
    
} // namespace Mix

#endif
