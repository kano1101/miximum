#pragma once

#include <cassert>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLU  // GLUライブラリを使用するのに必要
#include <GLFW/glfw3.h>

#include <loki/Singleton.h>

#include <mix/Utility.h>
#include "Input.h"


namespace Mix {

  ////////////////////////////////////////////////////////////////////////
  // InputGLの生成ポリシークラス
  
  template<class T>
  class InputGLCreator {
  public:
    static T* Create() {
      assert(window_);
      return new T(window_);
    }
    static void Destroy(T* c) {
      delete c;
    }
    static void Initialize(GLFWwindow* window) {
      window_ = window;
    }
  private:
    static GLFWwindow* window_;
  };

  template<class T>
  GLFWwindow* InputGLCreator<T>::window_ = nullptr;

  namespace Private {
  
    ////////////////////////////////////////////////////////////////////
    // GLFW利用入力デバイスまとめクラス
    class InputGL
      : public Mix::InputInterface
    {
    public:
      InputGL(GLFWwindow* const window)
        : window_(window) {
      }
      virtual bool IsPress(int key) override {
        return glfwGetKey(window_, key) == GLFW_PRESS;
      }
      virtual bool IsRelease(int key) override {
        return glfwGetKey(window_, key) == GLFW_RELEASE;
      }
      virtual bool IsRepeat(int key) override {
        return glfwGetKey(window_, key) == GLFW_REPEAT;
      }
    private:
      GLFWwindow* window_;
      MIX_LOKI_DEFINE_SINGLETON_CLASS(InputGL, InputGLCreator);
    };

  }

  namespace {

    using InputGL = Loki::SingletonHolder<Private::InputGL, InputGLCreator>;

    void InputGLSetup(GLFWwindow* window) {
      InputGLCreator<Private::InputGL>::Initialize(window);
      InputGL::Instance();
    }
    
  }
    
}

