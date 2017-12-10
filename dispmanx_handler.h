#ifndef RPI3_DISPMAX_HANDLER_H
#define RPI3_DISPMAX_HANDLER_H

#include <EGL/egl.h>
#include <bcm_host.h>

class DispmanxHandler {
public:
  void init();
  EGL_DISPMANX_WINDOW_T& native_window() { return native_window_; }
  DISPMANX_DISPLAY_HANDLE_T& dispman_display() { return dispman_display_; }
  uint32_t screen_width() const { return screen_width_; }
  uint32_t screen_height() const { return screen_height_; }

private:
  EGL_DISPMANX_WINDOW_T native_window_ {0, 0, 0};
  DISPMANX_DISPLAY_HANDLE_T dispman_display_ {0};
  uint32_t screen_width_ {0};
  uint32_t screen_height_ {0};
};

#endif