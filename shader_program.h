#ifndef RPI3_SHADER_PROGRAM_H
#define RPI3_SHADER_PROGRAM_H

#include <string>

#include <GLES2/gl2.h>

class ShaderProgram {
public:
  void init(const std::string& vertex_source, const std::string& fragment_source);
  void use();
  GLuint shader_id() const { return program_id_; }

private:
  GLuint program_id_;
};


#endif
