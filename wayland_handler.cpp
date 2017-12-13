#include "wayland_handler.h"

#include <cstring>

#include "logger.h"

WaylandHandler::~WaylandHandler() {
  wl_display_disconnect(display_);
}

void WaylandHandler::init() {
  get_server_references();
  surface_ = wl_compositor_create_surface(compositor_);
  if (surface_ == nullptr)
  {
    Logger::error("Wayland: Can't create surface");
  }
  else
  {
    Logger::info("Wayland: Surface created");
  }

  shell_surface_ = wl_shell_get_shell_surface(shell_, surface_);
  wl_shell_surface_set_toplevel(shell_surface_);

  create_opaque_region();
}

EGLNativeWindowType WaylandHandler::window() {
  egl_window_ = wl_egl_window_create(surface_, SCREEN_WIDTH, SCREEN_HEIGHT);
  if (egl_window_ == EGL_NO_SURFACE)
  {
    Logger::error("Can't create egl window");
  }
  else
  {
    Logger::info("Created egl window");
    return egl_window_;
  }
  return nullptr;
}

void WaylandHandler::get_server_references() {
    display_ = wl_display_connect(nullptr);
    if (display_ == nullptr)
    {
      Logger::error("Wayland: Can't connect to display");
    }
    else {
      Logger::info("Wayland: Connected to display");
    }

    wl_registry *registry = wl_display_get_registry(display_);
    wl_registry_add_listener(registry, &registry_listener, nullptr);

    wl_display_dispatch(display_);
    wl_display_roundtrip(display_);

    if (compositor_ == nullptr || shell_ == nullptr)
    {
      Logger::error("Wayland: Can't find compositor or shell");
    }
    else
    {
      Logger::info("Wayland: Found compositor and shell");
    }
}

void WaylandHandler::create_opaque_region()
{
    region_ = wl_compositor_create_region(compositor_);
    wl_region_add(region_, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    wl_surface_set_opaque_region(surface_, region_);
}

void WaylandHandler::registry_handler(void *data, wl_registry *registry, uint32_t id, const char *interface, uint32_t version)
{
    Logger::debug("Wayland: Got a registry event for %s id %d", interface, id);
    if (strcmp(interface, "wl_compositor") == 0)
    {
        compositor_ = reinterpret_cast<wl_compositor*>(wl_registry_bind(registry, id, &wl_compositor_interface, 1));
    }
    else if (strcmp(interface, "wl_shell") == 0)
    {
        shell_ = reinterpret_cast<wl_shell*>(wl_registry_bind(registry, id, &wl_shell_interface, 1));
    }
}

void WaylandHandler::registry_remover(void *data, wl_registry *registry, uint32_t id)
{
    Logger::debug("Wayland: Got a registry losing event for %d", id);
}

wl_compositor* WaylandHandler::compositor_ {nullptr};
wl_shell* WaylandHandler::shell_ {nullptr};
wl_registry_listener WaylandHandler::registry_listener = {
    registry_handler,
    registry_remover
  };