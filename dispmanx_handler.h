#ifndef RPI3_DISPMAX_HANDLER_H
#define RPI3_DISPMAX_HANDLER_H

#include <bcm_host.h>
#include <EGL/egl.h>

#include "egl_display_handler.h"

class DispmanxHandler : public EGLDisplayHandler {
public:
  void init() override;
  EGLNativeWindowType window() override { return reinterpret_cast<EGLNativeWindowType>(&native_window_); }
  //EGLNativeDisplayType display() override { return reinterpret_cast<EGLNativeDisplayType>(&dispman_display_); }
  EGLNativeDisplayType display() override { return EGL_DEFAULT_DISPLAY; }
  uint32_t screen_width() const override { return screen_width_; }
  uint32_t screen_height() const override { return screen_height_; }

  DispmanxHandler();
  ~DispmanxHandler();

private:
  EGL_DISPMANX_WINDOW_T native_window_ {0, 0, 0};
  DISPMANX_DISPLAY_HANDLE_T dispman_display_ {0};
  uint32_t screen_width_ {0};
  uint32_t screen_height_ {0};
};

#endif