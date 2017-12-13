#ifndef RPI3_WAYLAND_HANDLER_H
#define RPI3_WAYLAND_HANDLER_H

#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <wayland-egl.h>
#include <wayland-server.h>

#include "egl_display_handler.h"

class WaylandHandler : public EGLDisplayHandler {
public:
  void init() override;
  EGLNativeDisplayType display() override { return display_; };
  EGLNativeWindowType window() override;
  uint32_t screen_width() const override {return SCREEN_WIDTH;}
  uint32_t screen_height() const override {return SCREEN_HEIGHT;}

  void dispatch() override { wl_display_flush(display_);}

  ~WaylandHandler();

private:
  static const uint32_t SCREEN_WIDTH {480};
  static const uint32_t SCREEN_HEIGHT {360};

  static void registry_handler(void *data, wl_registry *registry, uint32_t id, const char *interface, uint32_t version);
  static void registry_remover(void *data, wl_registry *registry, uint32_t id);

  static wl_registry_listener registry_listener;

  void get_server_references();
  void create_opaque_region();

  static wl_compositor *compositor_;
  static wl_shell *shell_;

  wl_display *display_ {nullptr};
  wl_surface *surface_ {nullptr};
  wl_egl_window *egl_window_ {nullptr};
  wl_region *region_ {nullptr};
  wl_shell_surface *shell_surface_ {nullptr};
};

#endif