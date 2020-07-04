#pragma once

#include <loki/TypeTraits.h>
#include <mix/Utility.h>

namespace Mix {

  namespace Private {
    
    template<class T> struct Position { // Positionという名は変えれる
      virtual ~Position() {}
      Position(const T& pos) : obj_(pos) {}
      inline const T* Address() const { return &obj_; }
    private:
      const T obj_;
    };
    template<class T> struct Position<T*> {
      virtual ~Position() {}
      Position(const T* pos) : obj_(pos) {}
      inline const T* Address() const { return obj_; }
    private:
      const T* const obj_;
    };
    template<class T> struct Position<T&> {
      virtual ~Position() {}
      Position(const T& pos) : obj_(pos) {}
      inline const T* Address() const { return &obj_; }
    private:
      T& obj_;
    };
    
  }
    
  template<class Vector>
  class Shape {
  public:
    using RawVectorType = typename Convert2Raw<Vector>::Result;
    using LightweightVectorType = typename Convert2Lightweight<Vector>::Result;
    using ComponentType = typename RawVectorType::ComponentType;
    
    Shape(LightweightVectorType position)
      : position_(position) {
    }
    
    ComponentType X() const { return position_.Address()->X(); }
    ComponentType Y() const { return position_.Address()->Y(); }

  protected:
    const RawVectorType* Position() const {
      return position_.Address();
    }

  private:
    //    const Vector position_;
    Private::Position<Vector> position_;
  };

  // template<class Vector>
  // class Shape<Vector*> {
  // public:
  //   using ComponentType = typename Vector::ComponentType;
  //   using VectorType = Vector;

  //   Shape(Vector* const position)
  //     : position_(position) {
  //   }
  //   ComponentType X() const { return position_->X(); }
  //   ComponentType Y() const { return position_->Y(); }
    
  // protected:
  //   const VectorType* Position() { return position_; }

  // private:
  //   const Vector* const position_;
  // };

  // template<class Vector>
  // class Shape<Vector&> {
  // public:
  //   using ComponentType = typename Vector::ComponentType;
  //   using VectorType = Vector;

  //   Shape(const Vector& position)
  //     : position_(position) {
  //   }
  //   ComponentType X() const { return position_.X(); }
  //   ComponentType Y() const { return position_.Y(); }
    
  // protected:
  //   const VectorType* Position() { return &position_; }

  // private:
  //   const Vector& position_;
  // };
  
}

