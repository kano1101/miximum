#pragma once

#include <loki/TypeTraits.h>
#include <loki/MultiMethods.h>

#include "Rect.h"
#include "CollideObject.h"
#include "Vector.h"

namespace Mix {

  template<class Vector>
  class CollideRect
    : private Rect<Vector>
    , private CollideObject {
  public:
    using RawVectorType = typename Convert2Raw<Vector>::Result;
    using LightweightVectorType = typename Convert2Lightweight<Vector>::Result;
    using ComponentType = typename RawVectorType::ComponentType;

  public:
    virtual ~CollideRect() {}

  public:
    CollideRect(
      LightweightVectorType position,
      RawVectorType& size)
      : Rect<Vector>(position, size) {
    }
    CollideRect(
      LightweightVectorType position,
      const RawVectorType& first, const RawVectorType& last)
      : Rect<Vector>(position, first, last) {
    }

  public:
    using Rect<Vector>::X;
    using Rect<Vector>::Y;
    using Rect<Vector>::Left;
    using Rect<Vector>::Top;
    using Rect<Vector>::Right;
    using Rect<Vector>::Bottom;
    using Rect<Vector>::Width;
    using Rect<Vector>::Height;
    using Rect<Vector>::ResetTips;
    using Rect<Vector>::AssignBasePoint;
    using Rect<Vector>::MoveBasePoint;
  };

}

