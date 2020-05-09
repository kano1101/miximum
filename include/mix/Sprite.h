#ifndef _MIX_SPRITE_H_
#define _MIX_SPRITE_H_

#include <string>
#include <mix/Vector.h>
#include <mix/Actor.h>

namespace Mix
{

  class SpriteRenderer {
  public:
    virtual ~SpriteRenderer() {}
    virtual void Render(const std::string& fpath, const Vector2D& pos) = 0;
  };
  
  class Sprite2D
    : public Actor<SpriteRenderer>
  {
  public:
      
  public:
    Sprite2D(const std::string& fpath, Vector2D& pos)
      : fpath_(fpath), pos_(pos) {
    }
    virtual void Draw(SpriteRenderer& renderer) override {
      renderer.Render(fpath_, pos_);//2
    }
  private:
    std::string fpath_;
    Vector2D& pos_;
  };

} // namespace Mix

#endif
