#pragma once

#include <algorithm>

#include "TwinArray.h"

namespace Mix
{
  
  namespace Private {}
  namespace
  {
    
    namespace Private
    {
      
    } // namespace Private
    
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
//      using const_iterator         = typename container_type::const_iterator;
//      using reverse_iterator       = typename container_type::reverse_iterator;
//      using const_reverse_iterator = typename container_type::const_reverse_iterator;
//      using node_type;
//      using allocator_type;
      
    private:
      container_type c_;
      
    public:
      iterator begin() noexcept { return c_.begin(); }
//      const_iterator cbegin() const noexcept { return c_.cbegin(); }
      iterator end() noexcept { return c_.end(); }
//      const_iterator cend() const noexcept { return c_.cend(); }
      
//      reverse_iterator rbegin() noexcept { return c_.rbegin(); }
//      const_reverse_iterator crbegin const noexcept() { return c_.crbegin(); }
//      reverse_iterator rend() noexcept { return c_.rend(); }
//      const_reverse_iterator crend() const noexcept { return c_.crend(); }
      
    public:
      bool empty() const noexcept { return c_.empty(); }
      bool full() const noexcept { return c_.full(); }
      std::size_t max_size() const noexcept { return c_.max_size(); }
      std::size_t size() const noexcept { return c_.size(); }
      
    public:
//      void clear() noexcept { c_.clear(); }
//      template<class... Args>
//      std::pair<iterator, bool> emplace(Args&&... args) {}
//      template <class... Args>
//      iterator emplace_hint(const_iterator hint, Args&&... args) {}
//      void swap(multiset& st) {}
//      node_type extract(const_iterator position) {}
//      node_type extract(const key_type& x) {}
//      template<class C2>
//      void merge(set<Key, C2, Allocator>& source);       // (1)
//      template<class C2>
//      void merge(set<Key, C2, Allocator>&& source);      // (2)
//      template<class C2>
//      void merge(multiset<Key, C2, Allocator>& source);  // (3)
//      template<class C2>
//      void merge(multiset<Key, C2, Allocator>&& source); // (4)
      
    private:
      iterator lower_bound(const key_type& x) const noexcept
      {
        return std::lower_bound(c_.begin(), c_.end(), x, key_compare());
      }
//      iterator find_if(const key_type& x) const noexcept
//      {
//        return std::find_if(c_.begin(), c_.end(), [&](const key_type& y){ return x == y; });
//      }
      
    public:
      iterator insert(const key_type& x) noexcept                                           
      {
        return c_.insert(lower_bound(x), x);
      }
      iterator erase(iterator target) noexcept
      {
        return c_.erase(target);
      }
      
//      size_type count(const key_type& x) const
//      {
//        int result = 0;
//        for ( auto& e : c_ )
//        {
//          if (e == x) result++;
//        }
//        return result;
//      }
//
    }; // class MultiSetArray
    
  } // namespace
  
}
