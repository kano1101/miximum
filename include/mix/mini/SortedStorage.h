#ifndef __SORTED_STORAGE_H__
#define __SORTED_STORAGE_H__

#include <algorithm>
#include "MultiMapArray.h"
#include "Leaf.h"

namespace Mix
{

  namespace Private {}

  namespace {

    namespace Private {

      template<std::size_t MaxByte>
      class IntArrayStorage
      {
      public:
        using WorkType = int[(MaxByte + 3) / 4];
        
        void* operator & () noexcept { return static_cast<void*>(work_); }
        bool operator < (const IntArrayStorage & rhs) const noexcept
        { return work_ < rhs.work_; }
        
      private:
        WorkType   work_;
        
      }; // class IntArrayStorage

    } // namespace Private

    template<class TList>
    struct AscendingSortBy
    {
      template<class T>
      struct To
      {
        enum { value = Loki::TL::IndexOf<TList, T>::value };
      }; // struct To
        
    }; // struct AscendingSortBy
      
    template
    <
      std::size_t WorkCount,
      std::size_t WorkSize,
      class Storage = Private::IntArrayStorage<WorkSize>,
      class Container = MultiMapArray<int, Storage, WorkCount>
    >
    class SortedStorage
    {
    public:
      using container_type         = Container;

      using key_type               = int;
      using value_type             = Storage;
      using pair_type              = typename container_type::pair_type;
      
      using reference              = pair_type&;
      using const_reference        = const pair_type&;
      using pointer                = pair_type*;
      using const_pointer          = const pair_type*;
      
      using size_type              = std::size_t;
      using difference_type        = std::ptrdiff_t;
      
      using iterator               = typename container_type::iterator;

    public:
      // static std::uintptr_t GetMappedTypeOffset()
      // {
      //   static value_type v;
      //   static value_type v1;
      //   static iterator dummy(&v);
      //   static std::uintptr_t result =
      //     reinterpret_cast<std::uintptr_t>(&(dummy->second)) -
      //     reinterpret_cast<std::uintptr_t>(&(dummy));
      //   fprintf(stderr, "offset1                  :%d\n", result);
      //   fprintf(stderr, "offset2                  :%d\n", dummy.DiffTest());
      //   fprintf(stderr, "offset2 &(dummy)         :%p\n", &(dummy));
      //   fprintf(stderr, "offset2 &(dummy->second) :%p\n", &(dummy->second));
      //   return result;
      // }
      // iterator FindMappedTypeIterator(void* s)
      // {
      //   iterator result =
      //     std::find_if(
      //       c_.begin(),
      //       c_.end(),
      //       [&] (value_type& e) {
      //         return (&(e.second) == s);
      //       }
      //     );
      //     fprintf(stderr, "&(*result)=%p, &(result->first)=%p, &(result->second)=%p\n", &(*result), &(result->first), &(result->second));
      //     fprintf(stderr, "void* s = %p\n", s);
      //     void* testing = reinterpret_cast<void*>(
      //       reinterpret_cast<std::uintptr_t>(s) -
      //       reinterpret_cast<std::uintptr_t>(sizeof(typename value_type::first_type))
      //     );
      //     fprintf(stderr, "void* testing = %p\n", testing);
      //     fprintf(stderr, "result\n");
      //     result.TestPrint();
      //     fprintf(stderr, "check\n");
      //     iterator check(reinterpret_cast<value_type*>(testing));
      //     check.TestPrint();
          
      //   return result;
      // }

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
      static iterator Convert(void* head) noexcept
      {
        return container_type::ConvertToPairIterator(head);
      }
      
    };

    // template<std::size_t Count, std::size_t Size, class Storage, class Container>
    // constexpr value_type SortedStorage<Count, Size, Storage, Container>::dummy;
    
  } // namespace

} // namespace Mix
 
#endif
