#pragma once

#include "loki//TypeManip.h"

namespace Mix
{

  struct Dimension {};
  struct Dimension1D : Dimension { protected: double x_; };
  struct Dimension2D : Dimension1D { protected: double y_; };
  struct Dimension3D : Dimension2D { protected: double z_; };
  struct Dimension4D : Dimension3D { protected: double w_; };

  template<class>
  class Vector;

  typedef Vector<Dimension1D> Vector1D;
  typedef Vector<Dimension2D> Vector2D;
  typedef Vector<Dimension3D> Vector3D;
  typedef Vector<Dimension4D> Vector4D;

  template<class DimensionND>
  class Vector // 位置ベクトル (略しているが正確にはPositionVector)
    : public DimensionND
  {

  public:

    double X() const { return this->x_; }
    double Y() const { return this->y_; }
    double Z() const { return this->z_; }
    double W() const { return this->w_; }

    Vector AsX() const { return AsX(Loki::Type2Type<DimensionND>()); }
    Vector AsY() const { return AsY(Loki::Type2Type<DimensionND>()); }
    Vector AsZ() const { return AsZ(Loki::Type2Type<DimensionND>()); }
    Vector AsW() const { return AsW(Loki::Type2Type<DimensionND>()); }

    Vector1D AsX(Loki::Type2Type<Dimension1D>) const { return Vector1D(this->x_); }
    Vector2D AsX(Loki::Type2Type<Dimension2D>) const { return Vector2D(this->x_, 0.0); }
    Vector3D AsX(Loki::Type2Type<Dimension3D>) const { return Vector3D(this->x_, 0.0, 0.0); }
    Vector4D AsX(Loki::Type2Type<Dimension4D>) const { return Vector4D(this->x_, 0.0, 0.0, 0.0); }

    //Vector2D AsY(Loki::Type2Type<Dimension1D>) const { return Vector2D(this->x_, 0.0); }
    Vector2D AsY(Loki::Type2Type<Dimension2D>) const { return Vector2D(0.0, this->y_); }
    Vector3D AsY(Loki::Type2Type<Dimension3D>) const { return Vector3D(0.0, this->y_, 0.0); }
    Vector4D AsY(Loki::Type2Type<Dimension4D>) const { return Vector4D(0.0, this->y_, 0.0, 0.0); }

    //Vector3D AsZ(Loki::Type2Type<Dimension1D>) const { return Vector3D(this->x_, 0.0, 0.0); }
    //Vector3D AsZ(Loki::Type2Type<Dimension2D>) const { return Vector3D(this->x_, this->y_, 0.0); }
    Vector3D AsZ(Loki::Type2Type<Dimension3D>) const { return Vector3D(0.0, 0.0, this->z_); }
    Vector4D AsZ(Loki::Type2Type<Dimension4D>) const { return Vector4D(0.0, 0.0, this->z_, 0.0); }

    //Vector4D AsW(Loki::Type2Type<Dimension1D>) const { return Vector4D(this->x_, 0.0, 0.0, 0.0); }
    //Vector4D AsW(Loki::Type2Type<Dimension2D>) const { return Vector4D(this->x_, this->y_, 0.0, 0.0); }
    //Vector4D AsW(Loki::Type2Type<Dimension3D>) const { return Vector4D(this->x_, this->y_, this->z_, 0.0); }
    Vector4D AsW(Loki::Type2Type<Dimension4D>) const { return Vector4D(0.0, 0.0, 0.0, this->w_); }

    explicit Vector(double x = 0.0, double y = 0.0, double z = 0.0, double t = 0.0)
    {
      Initialize(x, y, z, t, Loki::Type2Type<DimensionND>());
    }

    void Initialize(double x, double y, double z, double t, Loki::Type2Type<Dimension1D>)
    {
      this->x_ = x;
    }
    void Initialize(double x, double y, double z, double t, Loki::Type2Type<Dimension2D>)
    {
      this->x_ = x; this->y_ = y;
    }
    void Initialize(double x, double y, double z, double t, Loki::Type2Type<Dimension3D>)
    {
      this->x_ = x; this->y_ = y; this->z_ = z;
    }
    void Initialize(double x, double y, double z, double t, Loki::Type2Type<Dimension4D>)
    {
      this->x_ = x; this->y_ = y; this->z_ = z; this->w_ = t;
    }

    void Reset(double x = 0.0, double y = 0.0, double z = 0.0, double t = 0.0) { this->Reset(Vector(x, y, z, t)); }
    void Move(double x = 0.0, double y = 0.0, double z = 0.0, double t = 0.0) { this->Move(Vector(x, y, z, t)); }

    void Reset(const Vector& v) { this->Reset(v, Loki::Type2Type<DimensionND>()); }
    void Move(const Vector& v) { this->Move(v, Loki::Type2Type<DimensionND>()); }

    void Reset(const Vector& v, Loki::Type2Type<Dimension1D>) { this->x_ = v.x_; }
    void Reset(const Vector& v, Loki::Type2Type<Dimension2D>) { this->x_ = v.x_; this->y_ = v.y_; }
    void Reset(const Vector& v, Loki::Type2Type<Dimension3D>) { this->x_ = v.x_; this->y_ = v.y_; this->z_ = v.z_; }
    void Reset(const Vector& v, Loki::Type2Type<Dimension4D>) { this->x_ = v.x_; this->y_ = v.y_; this->z_ = v.z_; this->w_ = v.w_; }

    void Move(const Vector& v, Loki::Type2Type<Dimension1D>) { this->x_ += v.x_; }
    void Move(const Vector& v, Loki::Type2Type<Dimension2D>) { this->x_ += v.x_; this->y_ += v.y_; }
    void Move(const Vector& v, Loki::Type2Type<Dimension3D>) { this->x_ += v.x_; this->y_ += v.y_; this->z_ += v.z_; }
    void Move(const Vector& v, Loki::Type2Type<Dimension4D>) { this->x_ += v.x_; this->y_ += v.y_; this->z_ += v.z_; this->w_ += v.w_; }

    void Inverse(bool x = false, bool y = false, bool z = false, bool t = false)
    {
      Inverse(x, y, z, t, Loki::Type2Type<DimensionND>());
    }

    void Inverse(bool x, bool y, bool z, bool t, Loki::Type2Type<Dimension1D>)
    {
      this->x_ *= x ? -1 : 1;
    }
    void Inverse(bool x, bool y, bool z, bool t, Loki::Type2Type<Dimension2D>)
    {
      this->x_ *= x ? -1 : 1; this->y_ *= y ? -1 : 1;
    }
    void Inverse(bool x, bool y, bool z, bool t, Loki::Type2Type<Dimension3D>)
    {
      this->x_ *= x ? -1 : 1; this->y_ *= y ? -1 : 1; this->z_ *= z ? -1 : 1;
    }
    void Inverse(bool x, bool y, bool z, bool t, Loki::Type2Type<Dimension4D>)
    {
      this->x_ *= x ? -1 : 1; this->y_ *= y ? -1 : 1; this->z_ *= z ? -1 : 1; this->w_ *= t ? -1 : 1;
    }

    double GetLength() const { return GetLength(Loki::Type2Type<DimensionND>()); }
    Vector GetInverse() const { return GetInverse(Loki::Type2Type<DimensionND>()); }
    Vector GetCompose(const Vector& v) const { return GetCompose(v, Loki::Type2Type<DimensionND>()); }
    Vector GetUnit() const { return GetUnit(GetLength(), Loki::Type2Type<DimensionND>()); }  //単位ベクトル生成

    double GetLength(Loki::Type2Type<Dimension1D>) const
    {
      return abs(this->x_);
    }
    double GetLength(Loki::Type2Type<Dimension2D>) const
    {
      return sqrt(this->x_ * this->x_ + this->y_ * this->y_);
    }
    double GetLength(Loki::Type2Type<Dimension3D>) const
    {
      return sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_);
    }
    double GetLength(Loki::Type2Type<Dimension4D>) const
    {
      return sqrt(this->x_ * this->x_ + this->y_ * this->y_ + this->z_ * this->z_ + this->w_ * this->w_);
    }

    Vector GetInverse(Loki::Type2Type<Dimension1D>) const { return Vector(-this->x_); }
    Vector GetInverse(Loki::Type2Type<Dimension2D>) const { return Vector(-this->x_, -this->y_); }
    Vector GetInverse(Loki::Type2Type<Dimension3D>) const { return Vector(-this->x_, -this->y_, -this->z_); }
    Vector GetInverse(Loki::Type2Type<Dimension4D>) const { return Vector(-this->x_, -this->y_, -this->z_, -this->w_); }

    Vector GetCompose(const Vector & v, Loki::Type2Type<Dimension1D>) const
    {
      return Vector(this->x_ + v.x_);
    }
    Vector GetCompose(const Vector & v, Loki::Type2Type<Dimension2D>) const
    {
      return Vector(this->x_ + v.x_, this->y_ + v.y_);
    }
    Vector GetCompose(const Vector & v, Loki::Type2Type<Dimension3D>) const
    {
      return Vector(this->x_ + v.x_, this->y_ + v.y_, this->z_ + v.z_);
    }
    Vector GetCompose(const Vector & v, Loki::Type2Type<Dimension4D>) const
    {
      return Vector(this->x_ + v.x_, this->y_ + v.y_, this->z_ + v.z_, this->w_ + v.w_);
    }

    Vector GetUnit(double len, Loki::Type2Type<Dimension1D>) const
    {
      return Vector(this->x_ / len);
    }
    Vector GetUnit(double len, Loki::Type2Type<Dimension2D>) const
    {
      return Vector(this->x_ / len, this->y_ / len);
    }
    Vector GetUnit(double len, Loki::Type2Type<Dimension3D>) const
    {
      return Vector(this->x_ / len, this->y_ / len, this->z_ / len);
    }
    Vector GetUnit(double len, Loki::Type2Type<Dimension4D>) const
    {
      return Vector(this->x_ / len, this->y_ / len, this->z_ / len, this->w_ / len);
    }

    double GetInner(const Vector2D & v) const { return (this->x_ * v.x_ + this->y_ * v.y_); }  //ベクトル内積計算
    double GetCross(const Vector2D & v) const { return (this->x_ * v.y_ - v.x_ * this->y_); }  //ベクトル外積計算

    Vector1D As1D() const { return As1D(Loki::Type2Type<DimensionND>()); }
    Vector2D As2D() const { return As2D(Loki::Type2Type<DimensionND>()); }
    Vector3D As3D() const { return As3D(Loki::Type2Type<DimensionND>()); }
    Vector4D As4D() const { return As4D(Loki::Type2Type<DimensionND>()); }

    Vector1D As1D(Loki::Type2Type<Dimension1D>) const { return Vector1D(this->x_); }
    Vector1D As1D(Loki::Type2Type<Dimension2D>) const { return Vector1D(this->x_); }
    Vector1D As1D(Loki::Type2Type<Dimension3D>) const { return Vector1D(this->x_); }
    Vector1D As1D(Loki::Type2Type<Dimension4D>) const { return Vector1D(this->x_); }

    Vector2D As2D(Loki::Type2Type<Dimension1D>) const { return Vector2D(this->x_, 0.0); }
    Vector2D As2D(Loki::Type2Type<Dimension2D>) const { return Vector2D(this->x_, this->y_); }
    Vector2D As2D(Loki::Type2Type<Dimension3D>) const { return Vector2D(this->x_, this->y_); }
    Vector2D As2D(Loki::Type2Type<Dimension4D>) const { return Vector2D(this->x_, this->y_); }

    Vector3D As3D(Loki::Type2Type<Dimension1D>) const { return Vector3D(this->x_, 0.0, 0.0); }
    Vector3D As3D(Loki::Type2Type<Dimension2D>) const { return Vector3D(this->x_, this->y_, 0.0); }
    Vector3D As3D(Loki::Type2Type<Dimension3D>) const { return Vector3D(this->x_, this->y_, this->z_); }
    Vector3D As3D(Loki::Type2Type<Dimension4D>) const { return Vector3D(this->x_, this->y_, this->z_); }

    Vector4D As4D(Loki::Type2Type<Dimension1D>) const { return Vector4D(this->x_, 0.0, 0.0, 0.0); }
    Vector4D As4D(Loki::Type2Type<Dimension2D>) const { return Vector4D(this->x_, this->y_, 0.0, 0.0); }
    Vector4D As4D(Loki::Type2Type<Dimension3D>) const { return Vector4D(this->x_, this->y_, this->z_, 0.0); }
    Vector4D As4D(Loki::Type2Type<Dimension4D>) const { return Vector4D(this->x_, this->y_, this->z_, this->w_); }


  public:
    // 演算子のオーバーロード
    Vector operator==(const Vector& v) const { return (this->x_ == v.x_) && (this->y_ == v.y_) && (this->z_ == v.z_) && (this->w_== v.w_); }

    Vector operator+() const { return *this; }
    Vector operator-() const { return this->GetInverse(); }

    Vector operator+=(const Vector& v) { this->Move(v); return (*this); }
    Vector operator-=(const Vector& v) { this->Move(v); return (*this); }

    Vector operator+(const Vector& rhs) const { return Vector(*this) += rhs; }
    Vector operator-(const Vector& rhs) const { return Vector(*this) -= rhs; }

  };



} // namespace Mix
