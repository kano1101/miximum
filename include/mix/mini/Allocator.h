//
// created by 狩野 亮 on 2020/03/29.
// Copyright (c) 2020年 狩野 亮. All rights reserved.
//

#ifndef __ERASE_SAFE_ALLOCATOR_H__
#define __ERASE_SAFE_ALLOCATOR_H__

#include <stdio.h>
#include <iterator>
#include <loki/Singleton.h>
#include <loki/Visitor.h>
#include "SortedStorage.h"

namespace Mix
{

  ///////////////////////////////////////////////////////////////////////
  // class A;
  // class SomeInterface { public: virtual void Method() = 0; };
  // using AllTList = Loki::TL::MakeTypelist<A>::Result;
  // const MemorySize = N;
  // using Base = BaseAllocatable<SomeInterface, AllTList, MemorySize>;
  // class A : public Base {
  // public:
  //   LOKI_DEFINE_CYCLIC_VISITABLE(ToStorageVisitor);
  //   virtual void Method() {}
  // };
  ///////////////////////////////////////////////////////////////////////
  template<class IF, class TList, std::size_t Size>
  class BaseAllocatable
    : public IF
  // , public Loki::BaseVisitable<IF*>
  {
  public:
    using Interface = IF;
    using AllTypelist = TList;
    enum { MaxCount = Size };

    virtual ~BaseAllocatable() {}
  };

  ////////////////////////////////////////////////////////////////////
  // ContainerLimitatioinPolicy

  template<typename Wrap>
  class Node
    : public {

  private:
    Node* prev_;
    Node* next_;
  };

  template<typename T, std::size_t N>
  class FixedLengthContainer {
  public:
    using Container = std::array<T, N>;
  };
  
  template<
    typename T,
    class LengthLimitationPolicy = FixedLengthContainer<T, N>,
    class StoragePlacementPolicy = StaticPoolOfMaxSize<TList>
    >
  class Allocator {
  public:
    void* allocate(std::size_t n);
    void deallocate(void* p);
    using BackEndType = LengthLimitationPolicy::Container;
  private:
    BackEndType backEnd_;
  };

  template<
    class OrderingPolicy = OrderedByIndex<TList>
    >
  class Inserter;

  /////////////////////////////////////////////////////////////////////////
  // SelfErasurePolicy　以下、ラフデザイン済

  template<class Iterator>
  void StepIf(const Iterator* target, const Iterator* shift) {
    if (shift == nullptr) assert(!"ポインタ参照が無効です。");
    if (std::equal_to<Iterator>()(*target, *shift)) ++(*shift);
  }

  template<class Iterator>
  class FastAlone {
  public:
    void Step(const Iterator* target) {
      ::Mix::StepIf(target, next_);
    }
  public:
    void Add(const Iterator* p) { // イテレータのコンストラクタで呼ぶ
    }
    void Remove(const Iterator* p) { // イテレータのデストラクタで呼ぶ
    }
  public:
    void Reset(const Iterator* p) {
      if (p != nullptr) next_ = p;
    }
  private:
    static Iterator* next_;;
  };
  template<class Iterator>
  Iterator* FastAlone<Iterator>::next_(nullptr);
    
  template<class Iterator>
  class AllIteratorStepIntoNext {
  public:
    void Step(const Iterator* target) { // インスタンスのremove時に呼ぶ
      std::for_each(
        monitored_.begin(),
        monitored_.end(),
        [&] (const Iterator* p) {
          ::Mix::StepIf(target, p);
        });
    }
  public:
    void Add(const Iterator* p) { // イテレータのコンストラクタで呼ぶ
      if (p != nullptr) monitored_.push_back(p);
    }
    void Remove(const Iterator* p) { // イテレータのデストラクタで呼ぶ
      if (p != nullptr) monitored_.remove(p);
    }
  public:
    void Reset(const Iterator* p) {
    }
  private:
    std::list<Iterator*> monitored_;
  };

  /////////////////////////////////////////////////////////////////////////////
  // ConversionPolicy

  class ReferenceResolution {
    
  };

  ///////////////////////////////////////////////////////////////////////////////
  // 
  
  template<class Storage>
  class Wrap {
  public:
    using Key = unsigned int;
    void* operator & () noexcept { return static_cast<void*>(&(pair_.second)); }
    bool operator < (const Wrap& rhs) const noexcept
    { return pair_ < rhs.pair_; }
  private:
    std::pair<const Key, Storage> pair_;
  };

  template<class Storage>
  class Wrap<Storage*> {
  public:
    void* operator & () noexcept { return static_cast<void*>(storage_); }
  private:
    Storage* storage_;
  };
  
  //////////////////////////////////////////////////////////////
  // OrderedPolicyのラフデザイン
  // Tにより適切なインデックスを求められること
  //////////////////////////////////////////////////////////////
 
  class Unordered {
  public:
    template<class T>
    struct Of {
      enum { value = 0 };
    };
  };

  class ManualOrder {
  };

  template<class TList>
  class OrderedByIndex {
  public:
    template<class T>
    struct Of {
      enum { value = Loki::TL::IndexOf<TList, T>::value };
    };
  private:
    unsigned int key_;
  };
  // struct less_inner {
  //   template < class T, class U >
  //   bool operator () ( const T &lhs, const U &rhs) const
  //   { return bool(lhs < rhs); }
  // };

  /////////////////////////////////////////////////////////////////////////
  // ユーティリティ：コンテナへのアクセスまとめ
  
  template<
    class Iterator
    class ConversionPolicy = ReferenceResolution,
    class SelfErasurePolicy = AllIteratorStepIntoNext
    >
  class Holder
    : public SelfErasurePolicy<Iterator> {
  public:
    template<class Iterator, class Function>
    void Each(Iterator first, Iterator last, Function f) {
      Reset(&first);
      for (;first != last;) {
        f(*first);
        Step(&first);
      }
    }
  };

  ///////////////////////////////////////////////////////////////////////////////////
  // 以下、未完成
  
  template<class TList, std::size_t WorkCount>
  struct ListedBy {
    
    template<class Container>
    using TheContainer = Loki::SingletonHolder<
      Container,
      Loki::CreateUsingNew,
p      Loki::PhoenixSingleton
      >;

    template<
      class TList,
      class SortingPolicy,
      class IterationPolicy
      >
    class Allocation;

    template<
      class T,
      std::size_t WorkSize = MaxSizeOf<TList>::value,
      class SortingPolicy = AscendingSortBy<TList>,
      template<class>class IterationPolicy = StaticErasableIteration,
      class Container = SortedStorage<WorkCount, WorkSize>
      >
    class Allocator
    {
    public:
      using container_type         = Container;

      using key_type               = typename container_type::key_type;
      using value_type             = typename container_type::value_type;
      using pair_type              = typename container_type::pair_type;
      
      using reference              = pair_type&;
      using const_reference        = const pair_type&;
      using pointer                = pair_type*;
      using const_pointer          = const pair_type*;
      
      using size_type              = std::size_t;
      using difference_type        = std::ptrdiff_t;
      
      using iterator               = typename container_type::iterator;

    private:
      using TheContainer           = TheContainer<container_type>;

    public:
      template<class Function>
      static Function ForAll(Function f)
      {
        iterator b = TheContainer::Instance().begin();
        iterator e = TheContainer::Instance().end();
        return ForEach<IterationPolicy>::Go(b, e, f);
      }

    public:
      class AllocatorInterface {
      public:
        virtual T* allocate(size_type) = 0;
        virtual void deallocate(T*) = 0;
      };

    public:
      static T* allocate(std::size_t n)
      {
        int prio = SortingPolicy::template To<T>::value;
        pair_type element(prio, {});
        auto iter = TheContainer::Instance().insert(element);
        void* head = &(iter->second);
        T* result = static_cast<T*>(head);
        return result;
      }
      static void deallocate(T* p, std::size_t n)
      {
        iterator pos = container_type::Convert(p);
        iterator next = TheContainer::Instance().erase(pos);
        // これによりGetNext()が返すイテレータは正しい要素を指すことになる
        IterationPolicy<iterator>::Set(next);
      }
      
    }; // class EraseSafeAllocator<T>

  };
    
} // namespace Mix

#endif
