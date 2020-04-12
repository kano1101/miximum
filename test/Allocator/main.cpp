#include <loki/Visitor.h>
#include <mix/Allocator.h>
#include <mix/Actor.h>

using namespace std;
using namespace Loki;
using namespace TL;
using namespace Mix;

class A;
class B;
class C;

using TList = MakeTypelist<A, B, C>::Result;

using ToStorageVisitor = CyclicVisitor<
  Actor*,
  TList
>;

class Base
  : public BaseAllocatable<Actor, TList, 16>
{
};

class A : public Base
{
public:
  LOKI_DEFINE_CYCLIC_VISITABLE(ToStorageVisitor);
  virtual void Update() override {}
  virtual void Draw() const override { cout << "A" << endl; }
};
class B : public Base
{
public:
  LOKI_DEFINE_CYCLIC_VISITABLE(ToStorageVisitor);
  virtual void Update() override {}
  virtual void Draw() const override { cout << "B" << endl; }
};
class C : public Base
{
public:
  LOKI_DEFINE_CYCLIC_VISITABLE(ToStorageVisitor);
  virtual void Update() override {}
  virtual void Draw() const override { cout << "C" << endl; }
};

class ToStorageVisitorImpl
  : public ToStorageVisitor
{
public:
  Actor* Visit(A& obj) { return &obj; }
  Actor* Visit(B& obj) { return &obj; }
  Actor* Visit(C& obj) { return &obj; }
};



int main()
{
  

  return 0;
}
