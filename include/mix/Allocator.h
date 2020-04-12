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
  
  namespace Private {}
  namespace
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

    namespace Private {
      template<class Container>
      using TheContainer = Loki::SingletonHolder<
        Container,
        Loki::CreateUsingNew,
        Loki::PhoenixSingleton
      >;
    }
    
    template<
      class T,
      std::size_t WorkCount = T::MaxCount,
      std::size_t WorkSize = MaxSizeOf<typename T::AllTypelist>::value,
      class SortingPolicy = AscendingSortBy<typename T::AllTypelist>,
      template<class>class IterationPolicy = StaticErasableIteration
    >
    class EraseSafeAllocator
    {
    private:
      using ContainerType = SortedStorage<WorkCount, WorkSize>;
      using TheContainer  = Private::TheContainer<ContainerType>;

    public:
      using Iterator    = typename ContainerType::iterator;
      using MappedType  = typename ContainerType::value_type;
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
        Iterator b = TheContainer::Instance().begin();
        Iterator e = TheContainer::Instance().end();
        return ForEach<IterationPolicy>::Go(b, e, f);
      }

    public:
      static T* allocate(std::size_t n)
      {
        int prio = SortingPolicy::template To<T>::value;
        MappedType element(prio, {});
        auto iter = TheContainer::Instance().insert(element);
        void* head = &(iter->second);
        T* result = static_cast<T*>(head);
        return result;
      }
      static void deallocate(T* p, std::size_t n)
      {
        // static const std::ptrdiff_t diff = TheContainer::Instance().end().Diff();
        // void* s = reinterpret_cast<void*>(reinterpret_cast<int64_t>(p) - diff);
        std::uintptr_t t = reinterpret_cast<std::uintptr_t>(p) - ContainerType::GetMappedTypeOffset();
        MappedType* s = reinterpret_cast<MappedType*>(t);
        Iterator pos(s);
        std::ptrdiff_t d =
          std::distance(TheContainer::Instance().begin(), TheContainer::Instance().end());
        if ( d != 0) fprintf(stderr, "%d\n", d);
        Iterator next = TheContainer::Instance().erase(pos);
        // これによりGetNext()が返すイテレータは正しい要素を指すことになる
        IterationPolicy<Iterator>::Set(next);
      }
      
    }; // class EraseSafeAllocator<T>
    
    
  } // namespace

} // namespace Mix

#endif
