#include "egl_handler.h"

#include <cassert>

#include "logger.h"

EGLHandler::EGLHandler() : state_(),
                           dispmanx_handler_()
{
  state_.display = EGL_NO_DISPLAY;
  state_.surface = EGL_NO_SURFACE;
  state_.context = EGL_NO_CONTEXT;
}

void EGLHandler::init()
{
  EGLint num_configs;
  EGLBoolean result;
  bcm_host_init();

  static const EGLint attribute_list[] =
      {
          EGL_RED_SIZE, 8,
          EGL_GREEN_SIZE, 8,
          EGL_BLUE_SIZE, 8,
          EGL_ALPHA_SIZE, 8,
          EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
          EGL_NONE};

  static const EGLint context_attributes[] =
      {
          EGL_CONTEXT_CLIENT_VERSION, 2,
          EGL_NONE};

  // get an EGL display connection
  state_.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  // initialize the EGL display connection
  result = eglInitialize(state_.display, NULL, NULL);
  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(state_.display, attribute_list, &state_.config, 1,
                           &num_configs);
  assert(EGL_FALSE != result);
  // Choose the OpenGL ES API
  result = eglBindAPI(EGL_OPENGL_ES_API);
  assert(EGL_FALSE != result);
  // create an EGL rendering context
  state_.context = eglCreateContext(state_.display,
                                    state_.config, EGL_NO_CONTEXT,
                                    context_attributes);
  assert(state_.context != EGL_NO_CONTEXT);

  dispmanx_handler_.init();
}

void EGLHandler::egl_from_dispmanx()
{
  EGLBoolean result;
  state_.surface = eglCreateWindowSurface(state_.display,
                                          state_.config,
                                          &dispmanx_handler_.native_window(),
                                          NULL);
  assert(state_.surface != EGL_NO_SURFACE);
  // connect the context to the surface
  result = eglMakeCurrent(state_.display, state_.surface, state_.surface, state_.context);
  assert(EGL_FALSE != result);
}

EGLHandler::~EGLHandler()
{
  if (state_.surface != EGL_NO_SURFACE &&
      eglDestroySurface(state_.display, state_.surface))
  {
    Logger::info("EGL surface destroyed ok");
  }
  if (state_.context != EGL_NO_CONTEXT &&
      eglDestroyContext(state_.display, state_.context))
  {
    Logger::info("EGL main context destroyed ok");
  }
  if (state_.display != EGL_NO_DISPLAY &&
      eglTerminate(state_.display))
  {
    Logger::info("EGL display terminated ok");
  }

  if (eglReleaseThread())
  {
    Logger::info("EGL thread resources released ok");
  }
  if (vc_dispmanx_display_close(dispmanx_handler_.dispman_display()) == 0)
  {
    Logger::info("Dispmanx display released ok");
  }
  bcm_host_deinit();
}
