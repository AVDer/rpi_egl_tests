#include "shader_program.h"

#include <iostream>

void ShaderProgram::init(const std::string& vertex_source, const std::string& fragment_source) {
  GLint success;
  GLchar infoLog[512];

  const GLchar* vertex_shader_code = vertex_source.c_str();
  const GLchar* fragment_shader_code = fragment_source.c_str();


  GLuint vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  GLuint fragment_shader { glCreateShader(GL_FRAGMENT_SHADER) };
  glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  program_id_ = glCreateProgram();
  glAttachShader(program_id_, vertex_shader);
  glAttachShader(program_id_, fragment_shader);

  // Bind vPosition to attribute 0
  glBindAttribLocation(program_id_, 0, "in_position");
  //glBindAttribLocation(program_id_, 1, "in_color");

  glLinkProgram(program_id_);

  glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(program_id_, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FIELD::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void ShaderProgram::use() {
  glUseProgram(program_id_);
}
