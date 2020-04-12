//
//  Created by 狩野 亮 on 2020/03/29.
//  Copyright © 2020年 狩野 亮. All rights reserved.
//

#ifndef __MULTISET_ARRAY_H__
#define __MULTISET_ARRAY_H__

#include <algorithm>

#include "TwinArray.h"

namespace Mix
{
  
  namespace Private {}
  namespace
  {
    
    template
    <
      typename Key,
      std::size_t MaxSize,
      class Compare = std::less<Key>,
      class Container = TwinArray<Key, MaxSize>
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
      iterator begin() noexcept { return c_.begin(); }
      iterator end() noexcept { return c_.end(); }
      
    public:
      bool empty() const noexcept { return c_.empty(); }
      bool full() const noexcept { return c_.full(); }
      std::size_t max_size() const noexcept { return c_.max_size(); }
      std::size_t size() const noexcept { return c_.size(); }
      
    private:
      iterator lower_bound(const key_type& x) const noexcept
      {
        return std::lower_bound(c_.begin(), c_.end(), x, key_compare());
      }
      
    public:
      iterator insert(const key_type& x) noexcept                                           
      {
        return c_.insert(lower_bound(x), x);
      }
      iterator erase(iterator target) noexcept
      {
        return c_.erase(target);
      }

    public:
      static iterator Convert(pointer p) noexcept
      {
        return container_type::ConvertToIterator(p);
      }
      
    }; // class MultiSetArray
    
  } // namespace
  
} // namespace Mix

#endif
