#include "egl_handler.h"

#include "logger.h"

EGLHandler::EGLHandler(std::shared_ptr<EGLDisplayHandler> display_handler) : state_(),
                           display_handler_(display_handler)
{
  state_.display = EGL_NO_DISPLAY;
  state_.surface = EGL_NO_SURFACE;
  state_.context = EGL_NO_CONTEXT;
}

void EGLHandler::init()
{
  EGLint num_configs;
  EGLBoolean result;

  display_handler_->init();

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
  state_.display = eglGetDisplay(display_handler_->display());
  // initialize the EGL display connection
  result = eglInitialize(state_.display, NULL, NULL);
  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(state_.display, attribute_list, &state_.config, 1,
                           &num_configs);
  if (result == EGL_FALSE) {
    Logger::error("EGL: Configuration choose fails");
  }
  // Choose the OpenGL ES API
  result = eglBindAPI(EGL_OPENGL_ES_API);
  if (result == EGL_FALSE) {
    Logger::error("EGL: EGL API binding fails");
  }
  // create an EGL rendering context
  state_.context = eglCreateContext(state_.display,
                                    state_.config, EGL_NO_CONTEXT,
                                    context_attributes);
  if (state_.context == EGL_NO_CONTEXT) {
    Logger::error("EGL: Context create fails");
  }
}

void EGLHandler::create_window()
{
  EGLBoolean result;
  state_.surface = eglCreateWindowSurface(state_.display,
                                          state_.config,
                                          display_handler_->window(),
                                          NULL);
  if (state_.surface == EGL_NO_SURFACE) {
    Logger::error("EGL: Surface create fails");
  }
  // connect the context to the surface
  result = eglMakeCurrent(state_.display, state_.surface, state_.surface, state_.context);
  if (result == EGL_FALSE) {
    Logger::error("EGL: Make current fails");
  }
}

EGLHandler::~EGLHandler()
{
  if (state_.surface != EGL_NO_SURFACE &&
      eglDestroySurface(state_.display, state_.surface))
  {
    Logger::debug("EGL: Surface destroyed ok");
  }
  if (state_.context != EGL_NO_CONTEXT &&
      eglDestroyContext(state_.display, state_.context))
  {
    Logger::debug("EGL: Main context destroyed ok");
  }
  if (state_.display != EGL_NO_DISPLAY &&
      eglTerminate(state_.display))
  {
    Logger::debug("EGL: Display terminated ok");
  }

  if (eglReleaseThread())
  {
    Logger::debug("EGL: Thread resources released ok");
  }
}
