#pragma once

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLU  // GLUライブラリを使用するのに必要
#include <GLFW/glfw3.h>

#include "Graphics.h"

namespace Mix {

////////////////////////////////////////////////////////////////////
  // GLFW利用レンダリング環境まとめクラス
  class GraphicsGL
    : public Mix::GraphicsInterface
  {
  public:
  GraphicsGL(GLFWwindow* const window, const int WIDTH, const int HEIGHT)
    : window_(window), WIDTH_(WIDTH), HEIGHT_(HEIGHT) {
      glDisable(GL_DEPTH_TEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glEnable(GL_ALPHA_TEST);
    }
    virtual void Begin() const override {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glOrtho(0.0, WIDTH_, HEIGHT_, 0.0, -1.0, 1.0);
    }
    virtual void End() const override {
      glfwSwapBuffers(window_);
    }
  private:
    GLFWwindow* const window_;
    const int WIDTH_, HEIGHT_;
  };

}

