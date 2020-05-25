#pragma once

#include <memory>
#include <loki/Functor.h>
#include <loki/Factory.h>

namespace Mix {

  template<class Product, typename IdentifierType, class ShareFunctor = Loki::Functor<std::shared_ptr<Product>>>
  class SharedFactory {
  public:
    typedef std::shared_ptr<Product> ReturnType;
    typedef Loki::AssocVector<IdentifierType, ShareFunctor> IdToProductMap;

  public:
    SharedFactory()
      : m_() {
    }
    ~SharedFactory() {
      m_.erase(m_.begin(), m_.end());
    }

    bool Register(const IdentifierType& id, ShareFunctor f) {
      return m_.insert(std::make_pair(id, f)).second != 0;
    }
    bool Unregister(const IdentifierType& id) {
      return m_.erase(id) != 0;
    }

    ReturnType GetSharedPointer(const IdentifierType& id) {
      typename IdToProductMap::iterator i = m_.find(id);
      if (i != m_.end())
        return (i->second)();
      assert(!"SharedFactory取得エラー");
    }

    std::size_t Count(const IdentifierType& id) {
      return static_cast<std::size_t>(m_.count(id));
    }
    
  private:
    IdToProductMap m_;
  };

  // template<class Product, typename  IdentifierType, class Loader = Loki::Functor<Product*>>
  // class SharedFactory {
  // public:
  //   //    using SharedProductPtr = std::shared_ptr<Product>;

  //   Product* CreateObject(const IdentifierType& id) {
  //     return factory_.CreateObject(id);
  //   }
  //   bool Register(const IdentifierType& id, Loader loader) {
  //     return factory_.Register(id, loader); // TODO: cx, cy へ初期値設定しなければならない
  //   }
  //   bool Unregister(const IdentifierType& id) {
  //     return factory_.Unregister(id);
  //   }
  // private:
  //   Loki::Factory<Product, IdentifierType, Loader> factory_;
  // };

}

