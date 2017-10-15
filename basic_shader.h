#ifndef RPI3_BASIC_SHADER_H
#define RPI3_BASIC_SHADER_H

#include <GLES2/gl2.h>

namespace BasicShader {

  static const GLchar *vertex_shader = R"(
  attribute vec3 in_position;
  attribute vec3 in_color;

  varying vec3 t_color;

  void main() {
    gl_Position = vec4(in_position, 1.0);
    t_color = in_color;
}
)";

  static const GLchar *fragment_shader = R"(
  varying vec3 t_color;

  void main() {
    gl_FragColor = vec4(t_color, 1.0);
  }
)";

};

#endif
