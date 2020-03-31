//
//  Created by 狩野 亮 on 2020/03/29.
//  Copyright © 2020年 狩野 亮. All rights reserved.
//

#ifndef __MIX_TEXTURE_H__
#define __MIX_TEXTURE_H__

namespace Mix
{

  class Texture
  {
  private:
    GLuint handle_;
    pngInfo info_;

  public:
    explicit Texture(const char* fnm)
    {
      LoadPNGFromeFile(fnm);
    }
    void LoadPNGFromeFile(const char* fnm)
    {
      handle_ = pngBind(fnm, PNG_NOMIPMAP, PNG_ALPHA, &info_, GL_CLAMP, GL_NEAREST, GL_NEAREST);
    }
    GLuint GetHandle() const noexcept { return handle_; }
    int GetWidth() const noexcept { return info_.Width; }
    int GetHeight() const noexcept { return info_.Height; }
  };
  
  template<std::size_t ChipCountX, std::size_t ChipCountY>
    class Sprite
    {
    private:
      std::vector<GLfloat*> src_;
      std::vector<GLfloat*> uv_;
    public:
      explicit Sprite(Texture& tex)
      {
	int w = tex.GetWidth();
	int h = tex.GetHeight();
	NewSourceRect(w, h);
      }
      ~Sprite()
	{
	  for ( auto& p : src_ )
	    delete[] p;
	  for ( auto& p : uv_ )
	    delete[] p;
	}
      void NewSourceRect(unsigned int w, unsigned int h)
      {
	GLfloat x = static_cast<GLfloat>(w  / ChipCountX);
	GLfloat y = static_cast<GLfloat>(h / ChipCountY);
	GLfloat u = static_cast<GLfloat>(1.0f / ChipCountX);
	GLfloat v = static_cast<GLfloat>(1.0f / ChipCountY);
	for ( unsigned int j = 0; j < ChipCountY; j++ )
	  {
	    for ( unsigned int i = 0; i < ChipCountX; i++ )
	      {
		// 0 - 1
		// |   |
		// 3 - 2
		GLfloat* p_vtx = new GLfloat[8] {
						 x * (i + 0), y * (j + 0),
						 x * (i + 1), y * (j + 0),
						 x * (i + 1), y * (j + 1),
						 x * (i + 0), y * (j + 1),
		};
		src_.push_back(p_vtx);

		// 0 - 1
		// |   |
		// 3 - 2
		GLfloat* p_uv = new GLfloat[8] {
						u * (i + 0), v * (j + 0),
						u * (i + 1), v * (j + 0),
						u * (i + 1), v * (j + 1),
						u * (i + 0), v * (j + 1),
		};
		uv_.push_back(p_uv);
	      }
	  }
      }
      void Draw(GLuint handle, GLfloat x, GLfloat y, int index = 0) const
      {
	{
	  glVertexPointer(2, GL_FLOAT, 0, src_[index]);
	  glTexCoordPointer(2, GL_FLOAT, 0, uv_[index]);

	  // テクスチャの描画
	  glEnable(GL_TEXTURE_2D);
	  glEnableClientState(GL_VERTEX_ARRAY);
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	  glPushMatrix();
	  glBindTexture(GL_TEXTURE_2D, handle);
	  glTranslatef(x, y, 0);
	  glDrawArrays(GL_QUADS, 0, 4);
	  glPopMatrix();

	  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	  glDisableClientState(GL_VERTEX_ARRAY);
	  glDisable(GL_TEXTURE_2D);
	}
      }
  
    }; // class Texture
  
} // namespace Mix

#endif
