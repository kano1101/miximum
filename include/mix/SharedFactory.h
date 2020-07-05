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
      if (i == m_.end())
        assert(!"SharedFactory取得エラー");
      return (i->second)();
    }

    std::size_t Count(const IdentifierType& id) {
      return static_cast<std::size_t>(m_.count(id));
    }
    
  private:
    IdToProductMap m_;
  };

}

