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

  // template<class StorageType>
  // using ToStorageVisitor = Loki::CyclicVisitor<
  //   StorageType*,
      

  template<class Iterator>
  class StaticErasableIteration
  {
  private:
    static Iterator next_;
      
  public:
    static void Set(Iterator next)
    {
      next_ = next;
    }
    static Iterator Get()
    {
      return next_;
    }
      
  }; // class StaticErasableIteration<Iterator>
    
  template<class Iterator>
  Iterator StaticErasableIteration<Iterator>::next_(nullptr);
    
  template<template<class>class IterationPolicy>
  struct ForEach
  {
    template<class Iterator, class Function>
    static Function Go(Iterator first, Iterator last, Function f)
    {
      using Iteration = IterationPolicy<Iterator>;
      Iteration::Set(first);
      for (;first != last;)
        {
          f(*first);
          // erase()されたか判定
          if (first == Iteration::Get())
            {
              // erase()されていない場合
              ++first;
              Iteration::Set(first);
            }
          else {
            // erase()されている場合
            first = Iteration::Get();
          }
        }
      return f;
    }
  };

  template<class Container>
  using TheContainer = Loki::SingletonHolder<
    Container,
    Loki::CreateUsingNew,
    Loki::PhoenixSingleton
  >;
    
  template<
    class T,
    std::size_t WorkCount = T::MaxCount,
    std::size_t WorkSize = MaxSizeOf<typename T::AllTypelist>::value,
    class SortingPolicy = AscendingSortBy<typename T::AllTypelist>,
    template<class>class IterationPolicy = StaticErasableIteration,
    class Container = SortedStorage<WorkCount, WorkSize>
  >
  class EraseSafeAllocator
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
    // using Interface   = typename T::Interface;

    // public:
    // static Iterator Convert(Interface* p)
    // {
    //   return Iterator(
    //     static_cast<StorageType*>(
    //       static_cast<std::uintptr_t>(p) -
    //       // static_cast<std::uintptr_t>(p->Accept(v)) -

    //       ContainerType::StorageOffset()
    //     )
    //   );
    // }
    // static Interface* Convert(Iterator i)
    // {
    //   return static_cast<Interface*>(&(i->second));
    // }
      
    template<class Function>
    static Function ForAll(Function f)
    {
      iterator b = TheContainer::Instance().begin();
      iterator e = TheContainer::Instance().end();
      return ForEach<IterationPolicy>::Go(b, e, f);
    }

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
      // static const std::ptrdiff_t diff = TheContainer::Instance().end().Diff();
      // void* s = reinterpret_cast<void*>(reinterpret_cast<int64_t>(p) - diff);

      // TheContainer::Instance().GetMappedTypeOffset();
      // fprintf(stderr, "T:%d, ", sizeof(T));
      // fprintf(stderr, "MappedType:%d, ", sizeof(MappedType));
      // fprintf(stderr, "first_type:%d, ", sizeof(typename MappedType::first_type));
      // fprintf(stderr, "second_type:%d, ", sizeof(typename MappedType::second_type));

      // MappedType element;
      // std::uintptr_t offset = &(element.first) - &(element.second);
      // MappedType* m = reinterpret_cast<MappedType*>(
      //   reinterpret_cast<std::uintptr_t>(p) -
      //   TheContainer::Instance().GetMappedTypeOffset()
      //   // offset
      // );
      // Iterator pos(m);
      iterator pos = container_type::Convert(p);
      iterator next = TheContainer::Instance().erase(pos);
      // これによりGetNext()が返すイテレータは正しい要素を指すことになる
      IterationPolicy<iterator>::Set(next);
    }
      
  }; // class EraseSafeAllocator<T>
    
} // namespace Mix

#endif
