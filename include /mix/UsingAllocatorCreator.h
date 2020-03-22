#pragma once

namespace Mix
{
  /////////////////////////////////////////////////////////////////
  // 生成ポリシー:ライブラリコード
  /////////////////////////////////////////////////////////////////
  template<template<class> class Alloc>
  struct CreateUsing
  {
    template<class T>
    struct Creator
    {
      template<class ...Args>
      static T* Create(Args ...args)
      {
        Alloc<T> allocator;
        void* p = allocator.allocate(1);
        return new(p) T(args...);
      }
      static void Destroy(T* p)
      {
        Alloc<T> allocator;
        if (p != nullptr)
        {
          p->~T();
          allocator.deallocate(p, 1);
        }
      }
    }; // struct Creator
    template<class T>
    struct NoDestroyCreator
    {
      template<class ...Args>
      static T* Create(Args ...args)
      {
        Alloc<T> allocator;
        void* p = allocator.allocate(1);
        return new(p) T(args...);
      }
      static void Destroy(T* p)
      {
      }
    }; // struct NoDestroyCreator

    /////////////////////////////////////////////////////////////////
    // デリータ:ライブラリコード
    // Mix::CreateUsing<Allocator>::Deleter<T>();
    /////////////////////////////////////////////////////////////////
    template<class T>
    struct Deleter
    {
      void operator()(T* p) const
      {
        Alloc<T> allocator;
        if (p != nullptr)
        {
          p->~T();
          allocator.deallocate(p, 1);
        }
      }
    }; // struct Deleter

  }; // struct CreateUsing

} // namespace Mix

