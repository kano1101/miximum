#ifndef _MIXIMUM_LEAF_H_
#define _MIXIMUM_LEAF_H_

#include <loki/Typelist.h>

namespace Miximum
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
    struct LeafLength<Loki::Typelist<Typelist<T1, T2>, U>>
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
    struct LeafIndexOf<Typelist<T, Tail>, T> // 繝槭ャ繝√ヱ繧ｿ繝ｼ繝ｳ(險井ｸ企幕蟋�)
    {
      enum { value = 0 };
    };
  template<class Head, class Tail, class T>
    struct LeafIndexOf<Loki::Typelist<Head, Tail>, T> // Head縺悟����隗｣縺ｧ縺阪↑縺�����ｴ蜷�
    {
      enum { value = Loki::TL::IndexOf<Loki::Typelist<Head, Tail>, T>::value };
    };
  template<class Head1, class Head2, class Tail, class T>
    struct LeafIndexOf<Loki::Typelist<Typelist<Head1, Head2>, Tail>, T> // Head縺悟����隗｣縺ｧ縺阪ｋ蝣ｴ蜷�
    {
    private:
      using Head = Loki::Typelist<Head1, Head2>; // Head縺後ち繧､繝励Μ繧ｹ繝�
      enum { temp = LeafIndexOf<Tail, T>::value }; //Tail縺ｯ蠢����★谺｡縺ｮTypelist縺起ullType
    public:
      enum { value = temp == -1 ? LeafIndexOf<Head, T>::value : LeafLength<Head>::value + temp };
    };

}

#endif

