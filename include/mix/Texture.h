#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GL/glpng.h>

 #include <mix/Vector.h>

namespace Mix {

  class Texture
  {
  private:
    GLuint id_;
    GLfloat* src_;
    std::vector<GLfloat*> uv_;
    
  public:
    Texture(GLuint id,
          unsigned int cx, // chip x-y
          unsigned int cy,
          unsigned int tw, // tex w-h
          unsigned int th)
      : id_(id)
    {
      GLfloat w = tw / cx; // chip w-h
      GLfloat h = th / cy;
    
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
    ~Texture() {
      delete[] src_;
      for(auto& p : uv_) delete[] p;
    }
    void Render(const Mix::Vector2D& pos) const//4
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
  
  class Loader {
  public:
    Loader(const std::string& fpath,
           unsigned int cx,
           unsigned int cy)
      : texture_(nullptr) {
      // const char* fpath = "mariwo.png";
      // const unsigned int cx = 1;
      // const unsigned int cy = 1;
      pngInfo pinf;
      GLuint handle = pngBind(
        fpath.c_str(),
        PNG_NOMIPMAP, PNG_ALPHA,
        &pinf,
        GL_CLAMP, GL_NEAREST, GL_NEAREST);
      std::cout << fpath << ", " << handle << std::endl;
      assert(handle && "PNGの読み込みに失敗しました。");
      texture_.reset(new Texture(
        handle, cx, cy,
        pinf.Width, pinf.Height));
    }
    ~Loader() {
      //      delete texture_;
    }
    std::shared_ptr<Texture> operator ()() const {
      assert(!!texture_ && "Textureオブジェクトがありません。");
      return texture_;
    }
  private:
    std::shared_ptr<Texture> texture_;
  };
  
}

