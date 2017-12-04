#ifndef RPI3_GL_TEXTURE_H
#define RPI3_GL_TEXTURE_H

#include <GLES2/gl2.h>

#include "tga_file.h"

using egl_image_t = void*;

enum class TextureSource {
  TGA,
  OMX
};

class Texture {
public:
  GLuint texture_id() const { return texture_id_; };
  egl_image_t egl_image() { return egl_image_; }

  explicit Texture(const TGAFile& texture_file)
  {
      uint8_t *pixels = texture_file.data();
      // Use tightly packed data
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      // Generate a texture object
      glGenTextures(1, &texture_id_);
      // Bind the texture object
      glBindTexture(GL_TEXTURE_2D, texture_id_);
      // Load the texture
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        texture_file.width(), texture_file.height(),
                   0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
      // Set the filtering mode
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }


  explicit Texture(egl_state_t* egl_state)
  {
      glGenTextures(1, &texture_id_);
      glBindTexture(GL_TEXTURE_2D, texture_id_);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      egl_image_ = eglCreateImageKHR(egl_state->display, egl_state->context, EGL_GL_TEXTURE_2D_KHR, reinterpret_cast<EGLClientBuffer>(texture_id_), 0);
      if (egl_image_ == EGL_NO_IMAGE_KHR)
      {
        Logger::error("Texture: eglCreateImageKHR failed");
      }
  }

private:
  GLuint texture_id_ {0};
  egl_image_t egl_image_ {nullptr};
};

#endif
