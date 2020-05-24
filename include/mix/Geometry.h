#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <loki/TypeTraits.h>
#include <loki/TypeManip.h>

#include "Vector.h"

namespace Mix
{

  class AbstractShape
  {
  public:
    virtual ~AbstractShape() {};
    virtual double GetArea() const = 0;
  };

  template<class VectorType>
  class PositionVector
    : public VectorType
  {
  public:
    using PositionType = VectorType;
    virtual ~PositionedObject() {}
  };

  class Rect
    : public AbstractShape
  {
  public:
    using VectorType = Vector2D;
    using FirstPositionType = PositionVector<VectorType>;
    using LastPositionType = PositionVector<VectorType>;

  protected:
    FirstPositionType first_;
    LastPositionType  last_;

  public:
    virtual ~Rect() {}
    Rect() : first_(), last_() {}

    virtual double GetArea() const override
    {
      return GetWidth() * GetHeight();
    }

    double GetWidth()  const { return last_.X() - first_.X(); }
    double GetHeight() const { return last_.Y() - first_.Y(); }
    double GetDepth()  const { return last_.Z() - first_.Z(); }

    double GetLeft()   const { return first_.X(); }
    double GetTop()    const { return first_.Y(); }
    double GetRight()  const { return last_.X(); }
    double GetBottom() const { return last_.Y(); }

    void Reset(const VectorType& first, const VectorType& last)
    {
      double l = (std::min)(first.X(), last.X());
      double t = (std::min)(first.Y(), last.Y());
      double r = (std::max)(first.X(), last.X());
      double b = (std::max)(first.Y(), last.Y());
      first_.Reeset(l, t);
      last_.Reset(r, b);
    }

  };

} // namespace Mix

