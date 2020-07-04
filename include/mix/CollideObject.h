#pragma once

namespace Mix {

  class CollideObject {
  public:
    virtual ~CollideObject() {}
    //    virtual bool IsCollide(const CollideObject&) = 0;
  };

  template<class CollideObjectT, class CollideObjectU>
  class Hatch;

}

