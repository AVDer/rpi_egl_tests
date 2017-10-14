#ifndef RPI3_DISPMAX_HANDLER_H
#define RPI3_DISPMAX_HANDLER_H

#include <EGL/egl.h>
#include <bcm_host.h>

class DispmanxHandler {
public:
  void init();
  EGL_DISPMANX_WINDOW_T& native_window() { return native_window_; }
  DISPMANX_DISPLAY_HANDLE_T& dispman_display() { return dispman_display_; }
private:
  EGL_DISPMANX_WINDOW_T native_window_;
  DISPMANX_DISPLAY_HANDLE_T dispman_display_;
};

#endif