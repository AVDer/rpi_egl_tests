#ifndef RPI3_EGL_HANDLER_H
#define RPI3_EGL_HANDLER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "dispmanx_handler.h"

struct egl_state_t
{
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    EGLConfig config;    
};

class EGLHandler {

public:
  void init();
  void egl_from_dispmanx();
  egl_state_t* state() { return &state_; }
  uint32_t screen_width() const { return dispmanx_handler_.screen_width(); }
  uint32_t screen_height() const { return dispmanx_handler_.screen_height(); }

  [[deprecated]] EGL_DISPMANX_WINDOW_T& native_window() { return dispmanx_handler_.native_window(); }
  [[deprecated]] DISPMANX_DISPLAY_HANDLE_T& dispman_display() { return dispmanx_handler_.dispman_display(); }

  EGLHandler();
  ~EGLHandler();

private:
  egl_state_t state_;
  DispmanxHandler dispmanx_handler_;
  
};

#endif