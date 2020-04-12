//
//  Created by 狩野 亮 on 2020/03/29.
//  Copyright © 2020年 狩野 亮. All rights reserved.
//

#ifndef _MIX_LEAF_H_
#define _MIX_LEAF_H_

#include <loki/Typelist.h>

namespace Mix
{
  template<class TList>
  struct LeafLength;

  template<>
  struct LeafLength<Loki::NullType>
  {
    enum { value = 0 };
  };
  template<class T, class U>
  struct LeafLength<Loki::Typelist<T, U>>
  {
    enum { value = 1 + LeafLength<U>::value };
  };
  template<class T1, class T2, class U>
  struct LeafLength<Loki::Typelist<Loki::Typelist<T1, T2>, U>>
  {
    enum { value = LeafLength<Loki::Typelist<T1, T2>>::value + LeafLength<U>::value };
  };

  template<class TList, class T>
  struct LeafIndexOf;

  template<class T>
  struct LeafIndexOf<Loki::NullType, T>
  {
    enum { value = -1 };
  };
  template<class Tail, class T>
  struct LeafIndexOf<Loki::Typelist<T, Tail>, T>
  {
    enum { value = 0 };
  };
  template<class Head, class Tail, class T>
  struct LeafIndexOf<Loki::Typelist<Head, Tail>, T>
  {
    enum { value = Loki::TL::IndexOf<Loki::Typelist<Head, Tail>, T>::value };
  };
  template<class Head1, class Head2, class Tail, class T>
  struct LeafIndexOf<Loki::Typelist<Loki::Typelist<Head1, Head2>, Tail>, T>
  {
  private:
    using Head = Loki::Typelist<Head1, Head2>;
    enum { temp = LeafIndexOf<Tail, T>::value };
  public:
    enum { value = temp == -1 ?
	   LeafIndexOf<Head, T>::value : LeafLength<Head>::value + temp };
  };

  template<class TList>
  struct MaxSizeOf
  {
    enum { value = 0 };
  };
      
  template<>
  struct MaxSizeOf<Loki::NullType>
  {
    enum { value = -1 };
  };
  template<class T>
  struct MaxSizeOf<Loki::Typelist<T, Loki::NullType>>
  {
    enum { value = sizeof(T) };
  };
  template<class T1, class T2>
  struct MaxSizeOf<Loki::Typelist<T1, T2>>
  {
    enum {
      value =
      (std::max)(
        static_cast<std::size_t>(sizeof(T1)),
        static_cast<std::size_t>(MaxSizeOf<T2>::value)
      )
    };
  };
      
}

#endif

