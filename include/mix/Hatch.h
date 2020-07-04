#pragma once

#include "CollideObject.h"
#include "CollideRect.h"

namespace Mix {

template<>
  class Hatch<CollideRect<Vector2D*>, CollideRect<Vector2D*>> {
  public:
    bool operator()(const CollideRect<Vector2D*>& lhs, const CollideRect<Vector2D*>& rhs) {
      if( lhs.Left() > rhs.Right()  ) return false;
      if( rhs.Left() > lhs.Right()  ) return false;
      if( lhs.Top()  > rhs.Bottom() ) return false;
      if( rhs.Top()  > lhs.Bottom() ) return false;
      return true;
    }
  };

  using HatchDispatcher = Loki::FunctorDispatcher<CollideObject, CollideObject, bool>;
  
  namespace {
    HatchDispatcher MakeHatchDispatcher() {
      using Rect = CollideRect<Vector2D*>;
      HatchDispatcher d;
      d.Add<Rect, Rect>(Hatch<Rect, Rect>());
      return d;
    }
  }

}

