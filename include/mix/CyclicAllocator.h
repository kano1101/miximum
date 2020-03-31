//
// created by 狩野 亮 on 2020/03/29.
// Copyright (c) 2020年 狩野 亮. All rights reserved.
//

#ifndef __CYCLIC_ALLOCATOR_H__
#define __CYCLIC_ALLOCATOR_H__

#include <loki/Singleton.h>

#include "SortedStorage.h"

namespace Mix
{
  
  namespace Private {}
  namespace
  {
    
    namespace Private
    {
      
      template<class TList, std::size_t WorkCount, template<class>class CreationPolicy>
      class SortedStorageForSingleton
        : public SortedStorage<TList, WorkCount>
      {
      private:
        SortedStorageForSingleton() = default;
        ~SortedStorageForSingleton() = default;

      public:
        SortedStorageForSingleton(const SortedStorageForSingleton&) = delete;
        SortedStorageForSingleton& operator = (const SortedStorageForSingleton&) = delete;
        SortedStorageForSingleton(SortedStorageForSingleton&&) = delete;
        SortedStorageForSingleton& operator = (SortedStorageForSingleton&&) = delete;

        friend CreationPolicy<SortedStorageForSingleton>;

      };
      
      template<class TList, std::size_t WorkCount>
      using TheSortedStorage = Loki::SingletonHolder
      <
        SortedStorageForSingleton<TList, WorkCount, Loki::CreateUsingNew>,
        Loki::CreateUsingNew,
        Loki::PhoenixSingleton
      >;
      
    } // namespace Private
    
    ///////////////////////////////////////////////////////////////////////
    // class A;
    // class SomeInterface { public: virtual void Method() = 0; };
    // using AllTList = Loki::TL::MakeTypelist<A>::Result;
    // const MemorySize = 1;
    // using Base = CyclicAllocatableBase<SomeInterface, AllTList, MemorySize>;
    // class A : public Base {};
    ///////////////////////////////////////////////////////////////////////
    template<class Interface, class TList, std::size_t Size>
    class BaseAllocatable
      : public Interface
    {
    public:
      using AllTypelist = TList;
      enum { MaxCount = Size };
      
    }; // BaseAllocatable<Interface, TList, Size>
    
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
//      void Step(Iterator crnt)
//      {
//        if (crnt == next_) ++next_;
//      }
      
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
      
    template<class T, template<class>class IterationPolicy = StaticErasableIteration>
    class CyclicAllocator
    {
    private:
      using The = Private::TheSortedStorage<typename T::AllTypelist, T::MaxCount>;
      using Iterator = typename The::ObjectType::iterator;
      
    public:
      template<class Function>
      static Function ForAll(Function f)
      {
        Iterator b = The::Instance().begin();
        Iterator e = The::Instance().end();
        return ForEach<IterationPolicy>::Go(b, e, f);
      }

    public:
      static T* allocate(std::size_t n)
      {
        int prio = The::ObjectType::OrderSorting::template To<T>::value;
        typename The::ObjectType::value_type piece(prio);
        auto iter = The::Instance().insert(piece);
        void* p = &(*iter);
        T* result = static_cast<T*>(p);
        return result;
      }
      static void deallocate(T* p, std::size_t n)
      {
        static const std::ptrdiff_t diff = The::Instance().end().Diff();
        void* pNode = reinterpret_cast<void*>(reinterpret_cast<int64_t>(p) - diff);
        
        Iterator next = The::Instance().erase(Iterator(pNode));
        // これによりGetNext()が返すイテレータは正しい要素を指すことになる
        IterationPolicy<Iterator>::Set(next);
        
        // ForEach<typename The::ObjectType::iterator>
      }
      
    }; // class CyclicAllocator<T>
    
    
  } // namespace

} // namespace Mix

#endif
