#include <GLES2/gl2.h>

#include "basic_shader.h"
#include "egl_handler.h"
#include "gl_texture.h"
#include "logger.h"
#include "omx_facade.h"
#include "shader_program.h"
#include "tga_file.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//void* video_decode(void* param);
void video_decode(egl_image_t egl_image);


int main(int /*argc*/, char ** /*argv*/)
{
  Logger::set_logging_level(Logger::LoggingLevel::LoggingTrace);
  Logger::trace("General: Main thread started");

  EGLHandler egl_handler;
  egl_handler.init();
  egl_handler.egl_from_dispmanx();

static const GLfloat BOX_SIZE {0.7f};

  GLfloat vertices[] = {

      // front

      -BOX_SIZE, -BOX_SIZE, -BOX_SIZE, 0.0f,  1.0f,
      -BOX_SIZE, +BOX_SIZE, -BOX_SIZE, 0.0f,  0.0f,
      +BOX_SIZE, +BOX_SIZE, -BOX_SIZE, 1.0f,  0.0f,
      +BOX_SIZE, +BOX_SIZE, -BOX_SIZE, 1.0f,  0.0f,
      +BOX_SIZE, -BOX_SIZE, -BOX_SIZE, 1.0f,  1.0f,
      -BOX_SIZE, -BOX_SIZE, -BOX_SIZE, 0.0f,  1.0f,

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
  //Texture textue(TGAFile("corsairs.tga"));
  Texture texture(egl_handler.state());
  GLuint texture_id = texture.texture_id();
  egl_image_t egl_image = texture.egl_image();
  std::thread omx_thread(video_decode, egl_image);
  omx_thread.detach();

  auto start = std::chrono::system_clock::now();
  float rotation {0.f};
  while (std::chrono::system_clock::now() - start < std::chrono::seconds(10))
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

    projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);
    view       = glm::translate(view, glm::vec3(0.f, 0.f, -2.f));
    model      = glm::rotate(trans, glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f));
    rotation += 1;
    trans = projection * view * model;

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(trans));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glFlush();
    eglSwapBuffers(egl_handler.state()->display, egl_handler.state()->surface);
  }

  return 0;
}

void video_decode(egl_image_t egl_image) {
  OMXFacade omx_facade;
  omx_facade.decode_to_egl("test.h264", egl_image);
}