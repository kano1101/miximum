#ifndef _MIX_SPRITE_H_
#define _MIX_SPRITE_H_

#include <vector>
#include <GL/glpng.h>

namespace Mix
{

  class Sprite2D
  {
  private:
    GLfloat* src_;
    std::vector<GLfloat*> uv_;
    GLuint id_;
    pngInfo pinf_;
    
  public:
    ~Sprite2D()
    {
      delete[] src_;
      for(auto& p : uv_) delete[] p;
    }
    Sprite2D(
      const char* fnm,
      unsigned int cx,
      unsigned int cy
    ) // TODO:アニメパターン情報
    {
      id_ = pngBind(fnm, PNG_NOMIPMAP, PNG_ALPHA, &pinf_, GL_CLAMP, GL_NEAREST, GL_NEAREST);
      
      GLfloat w = pinf_.Width / cx;
      GLfloat h = pinf_.Height / cy;
      src_ = new GLfloat[8] { 0, 0, w, 0, w, h, 0, h };

      GLfloat u = static_cast<GLfloat>(1.0f / cx);
      GLfloat v = static_cast<GLfloat>(1.0f / cy);
      for( unsigned int j = 0; j < cy; j++ )
        for( unsigned int i = 0; i < cx; i++ )
          uv_.push_back(new GLfloat[8] {
            u * (i + 0), v * (j + 0),
            u * (i + 1), v * (j + 0),
            u * (i + 1), v * (j + 1),
            u * (i + 0), v * (j + 1),
          });
    }

  public:
    void Draw(const Mix::Vector2D& pos) const
    {
      glVertexPointer(2, GL_FLOAT, 0, src_);
      glTexCoordPointer(2, GL_FLOAT, 0, uv_[0]); // TODO:アニメパターン情報に対する動作残し

      // テクスチャの描画
      glEnable(GL_TEXTURE_2D);
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      glPushMatrix();
      glBindTexture(GL_TEXTURE_2D, id_);
      glTranslatef(pos.X(), pos.Y(), 0);
      glDrawArrays(GL_QUADS, 0, 4);
      glPopMatrix();

      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisable(GL_TEXTURE_2D);

    }

  };

} // namespace Mix

#endif
