#pragma once

#include "loki/Singleton.h"

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
    
//    template<class InputIterator>
//    class ForEach
//    {
//    private:
//      InputIterator next_;
//
//    public:
//      template<class Function>
//      Function Go(InputIterator first, InputIterator last, Function f) const
//      {
//        Set(first);
//        for (;first != last;)
//        {
//          f(*first);
//          ++first;
//          if ( first != next_ ) first = next_;
//        }
//      }
//      void Set(InputIterator next)
//      {
//        next_ = next;
//      }
//      void Step()
//      {
//        ++next_;
//      }
//
//    }; // class ForEach<InputIterator>
//
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
      
    };
    
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
    
    

    
//    template<class T>
//    class Executor
//    {
//    private:
//      using The = Private::TheSortedStorage<typename T::AllTypelist, T::MaxCount>;
//
//    public:
//      template<class Function>
//      static Function ForAll(Function f)
//      {
//        return std::for_each(
//          The::Instance().begin(),
//          The::Instance().end(),
//          f);
//      }
//
//      template<class Function>
//      static Function ForAllWithErase(Function f)
//      {
//        for (auto it = The::Instance().begin();;)
//        {
//          auto next = it;
//          f(*it);
//          it++;
//          if ( it != next ) it = next;
//
//        }
//
//        return f;
//      }
//
//    };
    
  } // namespace

} // namespace Mix
