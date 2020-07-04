#pragma once

#include <algorithm>

#include <mix/Utility.h>

#include "Shape.h"

namespace Mix {

  template<class Vector>
  class RectCommon
    : private Shape<Vector> { // このVectorはポインタ型もしくは参照型かもしれない
  public:
    using Inherited = Shape<Vector>;
    using RawVectorType = typename Convert2Raw<Vector>::Result;
    // オブジェクトのコピーを生じさせないために生の型の場合参照へ変換する
    using LightweightVectorType = typename Convert2Lightweight<Vector>::Result;
    using ComponentType = typename RawVectorType::ComponentType;

  public:
    virtual ~RectCommon() {}
    
  public:
    RectCommon(LightweightVectorType position, const RawVectorType& size)
      : Inherited(position), first_(), last_(size) {
    }
    RectCommon(LightweightVectorType position, const RawVectorType& first, const RawVectorType& last)
      : Inherited(position), first_(first), last_(last) {
    }

  public:
    using Inherited::X;
    using Inherited::Y;

    ComponentType Left()   const { return X() + first_.X(); }
    ComponentType Top()    const { return Y() + first_.Y(); }
    ComponentType Right()  const { return X() + last_.X(); }
    ComponentType Bottom() const { return Y() + last_.Y(); }
    
    ComponentType Width()  const { return last_.X() - first_.X(); }
    ComponentType Height() const { return last_.Y() - first_.Y(); }

  public:
    // Shape<Vector>中の絶対座標position_は変えない
    void ResetTips(const RawVectorType& first, const RawVectorType& last) {
      ComponentType l = (std::min)(first.X(), last.X());
      ComponentType t = (std::min)(first.Y(), last.Y());
      ComponentType r = (std::max)(first.X(), last.X());
      ComponentType b = (std::max)(first.Y(), last.Y());
      first_.Reset(l, t);
      last_.Reset(r, b);
    }

  public:
    // Shape<Vector>内の絶対座標のもとを改める
    void AssignBasePoint(const RawVectorType& position) {
      *(Position()) = position;
    }
    // 相対頂点はそのままで基点を平行移動する
    void MoveBasePoint(const RawVectorType& amount) {
      *(Position()) += amount;
    }
    
  private:
    RawVectorType first_;
    RawVectorType last_;
    
  protected:
    using Inherited::Position;
    
  };
  
  template<class Vector>
  class Rect
    : private RectCommon<Vector>
  {
  public:
    using Inherited = RectCommon<Vector>;
    using RawVectorType = typename Convert2Raw<Vector>::Result;
    // オブジェクトのコピーを生じさせないために生の型の場合参照へ変換する
    using LightweightVectorType = typename Convert2Lightweight<Vector>::Result;
    using ComponentType = typename RawVectorType::ComponentType;

  public:
    virtual ~Rect() {}

  public:
    Rect(LightweightVectorType position, const RawVectorType& size)
      : Inherited(position, size) {
    }
    Rect(LightweightVectorType position, const RawVectorType& first, const RawVectorType& last)
      : Inherited(position, first, last) {
    }
    
  public:
    using Inherited::X;
    using Inherited::Y;
    
    using Inherited::Left;
    using Inherited::Top;
    using Inherited::Right;
    using Inherited::Bottom;
    
    using Inherited::Width;
    using Inherited::Height;

    using Inherited::ResetTips;
    using Inherited::AssignBasePoint;
    using Inherited::MoveBasePoint;

  private:
    using Inherited::Position;
    
  };

  // 基底クラスのもつ絶対座標VectorType* position_が外部のアドレスを参照する
  // 内部変数のfirst_を参照させて自己完結させることも可能だが簡略化のため却下
  // template<class Vector>
  // class Rect<Vector*>
  //   : private RectCommon<Vector*>
  // {
  // public:
  //   using Inherited = RectCommon<Vector*>;
  //   using VectorType = Vector;
  //   using ComponentType = typename VectorType::ComponentType;

  // public:
  //   virtual ~Rect() {}
    
  // public:
  //   Rect(VectorType* const position, const VectorType& size)
  //     : Inherited(position, size) {
  //   }
  //   Rect(VectorType* const position, const VectorType& first, const VectorType& last)
  //     : Inherited(position, first, last) {
  //   }
    
  // public:
  //   using Inherited::X;
  //   using Inherited::Y;
    
  //   using Inherited::Left;
  //   using Inherited::Top;
  //   using Inherited::Right;
  //   using Inherited::Bottom;
    
  //   using Inherited::Width;
  //   using Inherited::Height;

  //   using Inherited::ResetTips;
  //   using Inherited::AssignBasePoint;
  //   using Inherited::MoveBasePoint;

  // private:
  //   using Inherited::position_;

  // private:
  //   virtual VectorType* const ReferBasePoint() const override {
  //     return position_;
  //   }
  // };

}

