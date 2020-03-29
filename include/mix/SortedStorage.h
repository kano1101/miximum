#include "loki/Singleton.h"

#include "MultiSetArray.h"

namespace Mix
{
  
  namespace Private {}
  namespace
  {

    namespace Private
    {
      
      template<std::size_t MaxByte>
      class IntArrayStorage
      {
      public:
        using WorkType = int[(MaxByte + 3) / 4];
        
        void* operator & () noexcept { return static_cast<void*>(work_); }
        
      private:
        WorkType   work_;
        
      }; // class IntArrayStorage
      
      //////////////////////////////////////////////////////////////
      // óDêÊèáà î‰ärÉmÅ[Éh
      //////////////////////////////////////////////////////////////
      template<typename StorageType>
      class SortingPiece
      {
      private:
        std::size_t prio_;
        StorageType storage_;
        
      public:
        explicit SortingPiece(std::size_t prio = 0) noexcept : prio_(prio), storage_() {}
        void* operator & () { return &storage_; }
        bool operator == (const SortingPiece &rhs) const noexcept { return storage_ == rhs.storage_; }
        bool operator < (const SortingPiece &rhs) const noexcept { return prio_ < rhs.prio_; }
        
      }; // class SortingPiece
      
      template<typename StorageType>
      bool operator != (const SortingPiece<StorageType> &lhs, const SortingPiece<StorageType> &rhs)
      {
        return !(lhs == rhs);
      }
      template<typename StorageType>
      bool operator <= (const SortingPiece<StorageType> &lhs, const SortingPiece<StorageType> &rhs)
      {
        return !(lhs > rhs);
      }
      template<typename StorageType>
      bool operator > (const SortingPiece<StorageType> &lhs, const SortingPiece<StorageType> &rhs)
      {
        return (rhs < lhs);
      }
      template<typename StorageType>
      bool operator >= (const SortingPiece<StorageType> &lhs, const SortingPiece<StorageType> &rhs)
      {
        return (rhs <= lhs);
      }
      
      template<class TList>
      struct MaxSizeOf;
      
      template<>
      struct MaxSizeOf<Loki::NullType>
      {
        enum { value = -1 }; // É^ÉCÉvÉäÉXÉgÇ™ãÛÇ»ÇÁó·äO
      };
      template<class T>
      struct MaxSizeOf<Loki::Typelist<T, Loki::NullType>>
      {
        enum { value = sizeof(T) };
      };
      template<class T1, class T2>
      struct MaxSizeOf<Loki::Typelist<T1, T2>>
      {
        enum { value = (std::max)(static_cast<std::size_t>(sizeof(T1)), static_cast<std::size_t>(MaxSizeOf<T2>::value)) };
      };
      
      //////////////////////////////////////////////////////////////
      // í«â¡éûë}ì¸à íuåàíËÉÅÉ^ä÷êî ::valueÇ≈éÊìæ
      //////////////////////////////////////////////////////////////
      template<class TList>
      struct AscendingSortBy
      {
        // TÇÃTListÇ…Ç®ÇØÇÈï¿Ç—à íuÇ…åàíËÇ∑ÇÈÉ|ÉäÉVÅ[
        template<class T>
        struct To
        {
          enum { value = Loki::TL::IndexOf<TList, T>::value };
        }; // struct To
        
      }; // struct AscendingSortBy
      
      template<class TList>
      struct AlwaysPushFront
      {
        template<class T>
        struct To
        {
          enum { value = 0 };
        };
        
      }; // struct AlwaysPushFront
      
    } // namespace Private
    
    template<std::size_t Size>
    using SortingPieceSizedOf = Private::SortingPiece<Private::IntArrayStorage<Size>>;

    template
    <
      class TList, // ñ{ÉCÉìÉXÉ^ÉìÉXì‡ÉÅÉÇÉäóÃàÊÇ…äiî[Ç≥ÇÍÇ§ÇÈå^Ç∑Ç◊ÇƒÇä‹ÇﬁÉ^ÉCÉvÉäÉXÉg
      std::size_t WorkCount,
      std::size_t WorkSize = Private::MaxSizeOf<TList>::value,
      class SortingPolicy = Private::AscendingSortBy<TList>, // To<T>::value が定義されているポリシーメタ関数
      class Piece = SortingPieceSizedOf<WorkSize>,
      class Container = MultiSetArray<Piece, WorkCount>
    >
    class SortedStorage
    {
    public:
      using ProductList            = TList;
      using OrderSorting           = SortingPolicy;
      using PieceType              = Piece;
      
      using container_type         = Container;
      
      using value_type             = typename container_type::value_type;
      
      using reference              = value_type&;
      using const_reference        = const value_type&;
      using pointer                = value_type*;
      using const_pointer          = const value_type*;
      
      using size_type              = std::size_t;
      using difference_type        = std::ptrdiff_t;
      
      using iterator               = typename container_type::iterator;
//      using const_iterator         = typename container_type::const_iterator;
//      using reverse_iterator       = typename container_type::reverse_iterator;
//      using const_reverse_iterator = typename container_type::const_reverse_iterator;
//      using node_type;
//      using allocator_type;
      
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