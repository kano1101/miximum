#pragma once

#define GLFW_INCLUDE_GLU  // GLUライブラリを使用するのに必要
#include <GLFW/glfw3.h>

#include <mix/Framework.h>

namespace Mix {

  ////////////////////////////////////////////////////////////////////////
  // GLFW利用メインウィンドウ実装クラス
  class MainWindowGL
    : public Mix::Framework::MainWindowInterface
  {
  public:
    MainWindowGL(GLFWwindow* const window)
      : window_(window) {
    }
    virtual bool IsContinue() const override {
      return glfwWindowShouldClose(window_) == GL_FALSE;
    }
    virtual void CleanUp() const override {
      glfwPollEvents();
    }
    virtual float Now() const override {
      return glfwGetTime();
    }
  private:
    GLFWwindow* const window_;
  };

} // namespace Mix

