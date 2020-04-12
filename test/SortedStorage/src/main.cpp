#include <iostream>
#include <loki/Typelist.h>
#include <mix/SortedStorage.h>
#include <mix/Leaf.h>

using namespace std;
using namespace Loki;
using namespace TL;
using namespace Mix;

class Base
{
protected:
  int value_;
public:
  Base(int value) : value_(value) {}
  virtual void Print() = 0;

  bool operator < (const Base& rhs) const
  { return value_ < rhs.value_; }
};

class A : public Base
{
public:
  A(int value = 0) : Base(value) {}
  virtual void Print() { cout << "A::print(), value_=" << value_ << endl; }
};

class B : public Base
{
public:
  virtual void Print() { cout << "B::print()" << endl; }
};

using TList = MakeTypelist<A, B>::Result;
using Storage = SortedStorage<TList, 16, MaxSizeOf<TList>::value, A>;

Storage s;

int main()
{
  cout << "Test start." << endl;
  
  cout << "-----------------------------------" << endl;

  int id[6] = { 1, 2, 3, 5, 2, 3};
  A a[6] = { 2, 4, 6, 3, 5, 7};

  for (int i = 0; i < 6; i++)
    s.insert(std::pair<int, A>(id[i], a[i]));

  for( auto & e : s )
    e.second.Print();
  
  cout << "-----------------------------------" << endl;
  
  cout << "Test end." << endl;

  return 0;
}
