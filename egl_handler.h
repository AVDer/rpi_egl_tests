#ifndef RPI3_EGL_HANDLER_H
#define RPI3_EGL_HANDLER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <memory>

#include "egl_display_handler.h"

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
  void create_window();
  egl_state_t* state() { return &state_; }
  uint32_t screen_width() const { return display_handler_->screen_width(); }
  uint32_t screen_height() const { return display_handler_->screen_height(); }

  explicit EGLHandler(std::shared_ptr<EGLDisplayHandler> display_handler);
  ~EGLHandler();

private:
  egl_state_t state_;
  std::shared_ptr<EGLDisplayHandler> display_handler_;
  
};

#endif