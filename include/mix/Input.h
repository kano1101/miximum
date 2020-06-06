#pragma once

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLU  // GLUライブラリを使用するのに必要
#include <GLFW/glfw3.h>

namespace Mix {

  class InputInterface {
  public:
    virtual ~InputInterface() {}
    virtual bool IsPress(int key) = 0;
    virtual bool IsRelease(int key) = 0;
    virtual bool IsRepeat(int key) = 0;
  };
  
}
