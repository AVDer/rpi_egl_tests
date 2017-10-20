#include <GLES2/gl2.h>

#include "basic_shader.h"
#include "egl_handler.h"
#include "gl_texture.h"
#include "omx_facade.h"
#include "shader_program.h"
#include "tga_file.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int main(int /*argc*/, char ** /*argv*/)
{
  EGLHandler egl_handler;
  egl_handler.init();
  egl_handler.egl_from_dispmanx();

  GLfloat vertices[] = {

      // front
      //0
      -0.5f, -0.5f, 0.5f,
      0.0f, 0.5f,

      //1
      -0.5f, 0.5f, 0.5f,
      0.0f, 1.0f,

      //2
      0.5f, 0.5f, 0.5f,
      0.33f, 1.0f,

      //3
      0.5f, -0.5f, 0.5f,
      0.33f, 0.5f,

      // back
      //4
      -0.5f, -0.5f, -0.5f,
      0.0f, 0.0f,

      //5
      -0.5f, 0.5f, -0.5f,
      0.0f, 1.0f,

      //6
      0.5f, 0.5f, -0.5f,
      1.0f, 1.0f,

      //7
      0.5f, -0.5f, -0.5f,
      1.0f, 0.0f

  };

  GLubyte indecies[] = {
    //front
    0, 1, 2, 2, 3, 0,
    // right
    3, 2, 6, 6, 7, 3,
    // left
    4, 5, 1, 1, 0, 4,
    //back
    4, 5, 6, 6, 7, 4,
    // up
    1, 5, 6, 6, 2, 1,
    // down
    0, 4, 7, 7, 3, 0
  };

  //glViewport(0, 0, egl_handler.screen_width(), egl_handler.screen_height());
  glViewport(0, 0, 600, 600);

  ShaderProgram shader_program;
  shader_program.init(BasicShader::vertex_shader, BasicShader::fragment_shader);
  shader_program.use();

  glClearColor(0.0, 0.2, 0.2, 1.0);

  // Get locations
  GLint texture_location = glGetUniformLocation(shader_program.shader_id(), "u_texture");
  GLint mvp_location = glGetUniformLocation(shader_program.shader_id(), "u_mvp");

  // Load the texture
  Texture textue(TGAFile("test_texture.tga"));
  GLuint texture_id = textue.texture_id();

  glm::mat4 trans;

  for (auto i = 0; i < 100; ++i)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices + 3);
    glEnableVertexAttribArray(1);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // Set the sampler texture unit to 0
    glUniform1i(texture_location, 0);

    trans = glm::rotate(trans, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0));

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(trans));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indecies);

    glFlush();
    eglSwapBuffers(egl_handler.state()->display, egl_handler.state()->surface);

    usleep(100000);
  }

  //sleep(5);

  //OMXFacade omx_facade;
  //omx_facade.check_roles();

  return 0;
}
