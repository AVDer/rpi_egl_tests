#ifndef RPI3_DISPLAY_HANDLER_H
#define RPI3_DISPLAY_HANDLER_H

#include <EGL/egl.h>

class EGLDisplayHandler {
public:
  virtual void init() = 0;
  virtual EGLNativeDisplayType display() = 0;
  virtual EGLNativeWindowType window() = 0;
  virtual uint32_t screen_width() const  = 0;
  virtual uint32_t screen_height() const  = 0;

  virtual void dispatch() {}
};

#endif