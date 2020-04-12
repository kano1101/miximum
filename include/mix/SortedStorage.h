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

      // using value_type          = std::pair<int, Storage>;
      using value_type             = typename container_type::value_type;
      
      using reference              = value_type&;
      using const_reference        = const value_type&;
      using pointer                = value_type*;
      using const_pointer          = const value_type*;
      
      using size_type              = std::size_t;
      using difference_type        = std::ptrdiff_t;
      
      using iterator               = typename container_type::iterator;

    // private:
    //   constexpr static value_type dummy;
    //   constexpr static std::uintptr_t GetStorageOffset()
    //   {
    //     return (
    //       reinterpret_cast<std::uintptr_t>(&(dummy.second)) -
    //       reinterpret_cast<std::uintptr_t>(&dummy)
    //     );
    //   }

    // public:
    //   constexpr static std::uintptr_t offset_ = GetStorageOffset();

    public:
      static std::uintptr_t GetMappedTypeOffset()
      {
        static value_type dummy;
        static std::uintptr_t result =
          reinterpret_cast<std::uintptr_t>(&(dummy.second)) -
          reinterpret_cast<std::uintptr_t>(&dummy);
        return result;
      }

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
      
    };

    // template<std::size_t Count, std::size_t Size, class Storage, class Container>
    // constexpr value_type SortedStorage<Count, Size, Storage, Container>::dummy;
    
  } // namespace

} // namespace Mix
 
#endif
