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

GLuint gl_field_vbo;
GLuint gl_field_vao;
GLuint gl_field_ebo;

int main(int /*argc*/, char ** /*argv*/)
{
  EGLHandler egl_handler;
  egl_handler.init();
  egl_handler.egl_from_dispmanx();

static const GLfloat BOX_SIZE {0.7f};

  GLfloat vertices[] = {

      // front

      -BOX_SIZE, -BOX_SIZE, -BOX_SIZE, 0.0f,  0.0f,
      -BOX_SIZE, +BOX_SIZE, -BOX_SIZE, 0.0f,  1.0f,
      +BOX_SIZE, +BOX_SIZE, -BOX_SIZE, 1.0f,  1.0f,
      +BOX_SIZE, +BOX_SIZE, -BOX_SIZE, 1.0f,  1.0f,
      +BOX_SIZE, -BOX_SIZE, -BOX_SIZE, 1.0f,  0.0f,
      -BOX_SIZE, -BOX_SIZE, -BOX_SIZE, 0.0f,  0.0f,

    };
/*
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
*/

  glViewport(0, 0, egl_handler.screen_width(), egl_handler.screen_height());

  ShaderProgram shader_program;
  shader_program.init(BasicShader::vertex_shader, BasicShader::fragment_shader);
  shader_program.use();

  glClearColor(0.0, 0.2, 0.2, 1.0);

  // Get locations
  GLint texture_location = glGetUniformLocation(shader_program.shader_id(), "u_texture");
  GLint mvp_location = glGetUniformLocation(shader_program.shader_id(), "u_mvp");

  // Load the texture
  Texture textue(TGAFile("corsairs.tga"));
  GLuint texture_id = textue.texture_id();

    for (auto i = 0; i < 10; ++i)
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

    glm::mat4 trans;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;

    //projection = glm::perspective(glm::radians(45.0f), (float)egl_handler.screen_width() / (float)egl_handler.screen_height(), 0.1f, 100.0f);
    //view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.f);
    //model      = glm::rotate(trans, glm::radians(3.0f), glm::vec3(0.0, 1.0, 0.0));
    trans = projection * view * model;

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(trans));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glFlush();
    eglSwapBuffers(egl_handler.state()->display, egl_handler.state()->surface);

    usleep(100000);
  }

  //sleep(5);

  OMXFacade omx_facade;
  omx_facade.check_roles();
  omx_facade.get_port_info("OMX.broadcom.image_decode", 320);

  return 0;
}
