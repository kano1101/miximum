#include <iostream>

#include <loki/Typelist.h>
#include <loki/HierarchyGenerators.h>

#include <mix/Leaf.h>

using namespace std;

using namespace Loki;
using namespace TL;

using namespace Mix;

using T1 = Int2Type<1>;
using T2 = Int2Type<2>;
using T3 = Int2Type<3>;
using T4 = Int2Type<4>;
using T5 = Int2Type<5>;
using T6 = Int2Type<6>;
using T7 = Int2Type<7>;
using T8 = Int2Type<8>;
using T9 = Int2Type<9>;

using TList1 = MakeTypelist<T1, T2, T3>::Result;
using TList2 = MakeTypelist<T4, T5, T6>::Result;
using TList3 = MakeTypelist<T7, T8, T9>::Result;
using TList = MakeTypelist<TList1, TList2, TList3>::Result;

int main()
{
  cout << "Test start." << endl;
  
  cout << "-----------------------------------" << endl;

  cout << "LeafLength<TList>::value = " << LeafLength<TList>::value << endl;

  cout << "-----------------------------------" << endl;
  
  cout << "LeafLength<TList1>::value = " << LeafLength<TList1>::value << endl;
  cout << "LeafLength<TList2>::value = " << LeafLength<TList2>::value << endl;
  cout << "LeafLength<TList3>::value = " << LeafLength<TList3>::value << endl;
  
  cout << "-----------------------------------" << endl;

  cout << "LeafIndexOf<TList, T1>::value = " << LeafIndexOf<TList, T1>::value << endl;
  cout << "LeafIndexOf<TList, T2>::value = " << LeafIndexOf<TList, T2>::value << endl;
  cout << "LeafIndexOf<TList, T3>::value = " << LeafIndexOf<TList, T3>::value << endl;
  cout << "LeafIndexOf<TList, T4>::value = " << LeafIndexOf<TList, T4>::value << endl;
  cout << "LeafIndexOf<TList, T5>::value = " << LeafIndexOf<TList, T5>::value << endl;
  cout << "LeafIndexOf<TList, T6>::value = " << LeafIndexOf<TList, T6>::value << endl;
  cout << "LeafIndexOf<TList, T7>::value = " << LeafIndexOf<TList, T7>::value << endl;
  cout << "LeafIndexOf<TList, T8>::value = " << LeafIndexOf<TList, T8>::value << endl;
  cout << "LeafIndexOf<TList, T9>::value = " << LeafIndexOf<TList, T9>::value << endl;

  cout << "-----------------------------------" << endl;
  
  cout << "LeafIndexOf<TList, TList>::value = " << LeafIndexOf<TList, TList>::value << endl;

  //  cout << "-----------------------------------" << endl;

  //  cout << "LeafIndexOf<TList, TList1>::value = " << LeafIndexOf<TList, TList1>::value << endl;
  //  cout << "LeafIndexOf<TList, TList2>::value = " << LeafIndexOf<TList, TList2>::value << endl;
  //  cout << "LeafIndexOf<TList, TList3>::value = " << LeafIndexOf<TList, TList3>::value << endl;

  cout << "-----------------------------------" << endl;

  cout << "LeafIndexOf<TList, NullType>::value = " << LeafIndexOf<TList, NullType>::value << endl;
  
  cout << "-----------------------------------" << endl;

  cout << "Test end." << endl;

  return 0;
}
