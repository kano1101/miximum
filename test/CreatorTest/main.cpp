#include <iostream>
#include <random>

#include <loki/Typelist.h>
#include <mix/Director.h>


class Actor {
public:
  virtual ~Actor() {}
  virtual void Draw() const = 0;
};

template<class Policy>
class GameObject
  : public Actor {
public:
  virtual void Draw() const { std::cout << typeid(*this).name() << "::" << __func__ << std::endl; }
};

using A = GameObject<Loki::Int2Type<0>>;
using B = GameObject<Loki::Int2Type<1>>;
using C = GameObject<Loki::Int2Type<2>>;

using TList = Loki::TL::MakeTypelist<A, B, C>::Result;

template<class T>
using Allocator = Mix::Director<TList>::Allocator<T>;
template<class T>
using Creator = Mix::CreateUsing<Allocator>::Creator<T>;

using Executor = Mix::Director<TList>::Executor<Actor>;

int main()
{
  std::list<Actor*> lstA;
  std::list<Actor*> lstB;
  std::list<Actor*> lstC;
  
  std::mt19937 mt{ std::random_device{}() };
  std::uniform_int_distribution<int> dist(1, 3);

  clock_t t1, t2, t3, td1, td2, tt1, tt2, tb;
  tb = CLOCKS_PER_SEC;
  tt1 = 0;
  tt2 = 0;

  for ( int k = 0; k < 5; ++k ) {
    t1 = clock();

    for ( int i = 0 ; i < 10000; ++i ) {
      switch(dist(mt)) {
      case 1: lstA.push_back(Creator<A>::Create()); break;
      case 2: lstB.push_back(Creator<B>::Create()); break;
      case 3: lstC.push_back(Creator<C>::Create()); break;
      }
    }

    t2 = clock();
    //    Executor::each([](auto& e){ e.Draw(); });

    for (auto p : lstA) Creator<Actor>::Destroy(p);
    for (auto p : lstB) Creator<Actor>::Destroy(p);
    for (auto p : lstC) Creator<Actor>::Destroy(p);
    
    lstA.clear();
    lstB.clear();
    lstC.clear();
    
    t3 = clock();

    td1 = t2 - t1;
    td2 = t3 - t2;
    
    tt1 += td1;
    tt2 += td2;
  
    std::cout << "経過秒：生成" << static_cast<double>(td1) / static_cast<double>(tb) << ", 破棄" << static_cast<double>(td2) / static_cast<double>(tb) << std::endl;
  }
  std::cout << "合計：生成" << static_cast<double>(tt1) / static_cast<double>(tb) << ", 破棄" << static_cast<double>(tt2) / static_cast<double>(tb) << std::endl;

  Performance::Instance().Print();

  return 0;
}
