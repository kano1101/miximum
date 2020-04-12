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
  
  namespace Private {}
  namespace
  {

    namespace Private
    {
      
      class Priority
      {
        int value_;
      public:
        explicit Priority(int value) : value_(value) {}
        bool operator == (const Priority &rhs) const noexcept
        { return value_ == rhs.value_; }
        bool operator < (const Priority &rhs) const noexcept
        { return value_ < rhs.value_; }
      };
        
      
      bool operator != (const Priority &lhs, const Priority &rhs)
      {
        return !(lhs == rhs);
      }
      bool operator > (const Priority &lhs, const Priority &rhs)
      {
        return (rhs < lhs);
      }
      bool operator <= (const Priority &lhs, const Priority &rhs)
      {
        return !(lhs > rhs);
      }
      bool operator >= (const Priority &lhs, const Priority &rhs)
      {
        return (rhs <= lhs);
      }
      
      struct less_inner {
        template < class T, class U >
        bool operator () ( const T &lhs, const U &rhs) const
        { return bool(lhs < rhs); }
      };

    } // namespace Private
    
    template
    <
      typename Key,
      typename T,
      std::size_t WorkCount,
      class Compare = Private::less_inner,
      class Container = MultiSetArray<std::pair<const Key, T>, WorkCount, Compare>
    >
    class MultiMapArray
    {
    public:
      using key_type               = Key;
      using value_type             = std::pair<const Key, T>;
      using mapped_type            = T;

      using key_compare            = Compare;
      
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
      iterator insert(const value_type& x) noexcept
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
      
    }; // class SortedStorage
    
  } // namespace
  
} // namespace Mix

#endif
