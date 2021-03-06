#pragma once

#include <memory>

namespace Mix
{
  /////////////////////////////////////////////////////////////////
  // 生成ポリシー:ライブラリコード
  /////////////////////////////////////////////////////////////////
  template<template<class> class Alloc>
  struct CreateUsing {
    template<class T>
    class Creator {
    public:
      template<class ...Args>
      static T* Create(Args ...args) {
        Alloc<T> allocator;
      
        using traits = std::allocator_traits<Alloc<T>>;
        T* p = traits::allocate(allocator, 1);
        traits::construct(allocator, p, args...);
      
        return p;
      }
      static void Destroy(T* p) {
        if (p == nullptr) return;
        Alloc<T> allocator;
      
        using traits = std::allocator_traits<Alloc<T>>;
        traits::destroy(allocator, p);
        traits::deallocate(allocator, p, 1);
      }
    };

    template<class T>
    struct NoDestroyCreator
    {
      template<class ...Args>
      static T* Create(Args ...args) {
        Alloc<T> allocator;
      
        using traits = std::allocator_traits<Alloc<T>>;
        T* p = traits::allocate(allocator, 1);
        traits::construct(allocator, p, args...);
      
        return p;
      }
      static void Destroy(T* p) {
        (void) p;
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
        if (p == nullptr) return;
        Alloc<T> allocator;
      
        using traits = std::allocator_traits<Alloc<T>>;
        traits::destroy(allocator, p);
        traits::deallocate(allocator, p, 1);
      }
    }; // struct Deleter

  }; // struct CreateUsing

} // namespace Mix

