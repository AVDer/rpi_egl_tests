#ifndef RPI3_BASIC_SHADER_H
#define RPI3_BASIC_SHADER_H

#include <GLES2/gl2.h>

namespace BasicShader {

  static const GLchar *vertex_shader = R"(
  attribute vec3 in_position;
  attribute vec2 in_tex_coord;

  varying vec2 t_tex_coord;

  void main() {
    gl_Position = vec4(in_position, 1.0);
    t_tex_coord = in_tex_coord;
}
)";

  static const GLchar *fragment_shader = R"(
    varying vec2 t_tex_coord;

    uniform sampler2D u_texture;

  void main() {
    //gl_FragColor = vec4(t_color, 1.0);
    gl_FragColor =  texture2D(u_texture, t_tex_coord);    
  }
)";

};

#endif
