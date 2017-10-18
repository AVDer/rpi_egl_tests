#ifndef RPI3_GL_TEXTURE_H
#define RPI3_GL_TEXTURE_H

#include <GLES2/gl2.h>

#include "tga_file.h"

class Texture {
public:
  GLuint texture_id() const { return texture_id_; };

  Texture(const TGAFile& texture_file)
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

private:
  GLuint texture_id_ {0};
};

#endif
