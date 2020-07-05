#pragma once

#include <iostream>
#include <string>
#include <loki/Singleton.h>
#include <mix/SharedFactory.h>
#include <mix/Texture.h>
#include <mix/Vector.h>

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

namespace Mix {

  namespace Private {
    using TextureSharer =
      Loki::SingletonHolder<SharedFactory<Texture, std::string, Loader>>; // ここのLoaderを忘れると動作しない
  }
  
  class Sprite {
  public:
    Sprite(const std::string& fpath, unsigned int cx, unsigned int cy, const Vector2D& pos)
      : fpath_(fpath), pos_(pos) {
      auto& factory = Private::TextureSharer::Instance();
      auto count = factory.Count(fpath_);
      if ( count ) return;
      Loader loader = Loader(fpath, cx, cy); // TODO:LoaderはSingletonにしなければならないかも
      // 関数オブジェクトLoader登録：解放すべきインスタンス保持される
      bool result = factory.Register(fpath_, loader);
      UNUSED(result);
      assert(result && "画像データオブジェクトの登録システムエラー");
    }
    virtual void Draw() const {
      Private::TextureSharer::Instance().GetSharedPointer(fpath_)->Render(pos_);
    }
  private:
    const std::string fpath_;
    const Vector2D& pos_;
  };

} // namespace Mix

