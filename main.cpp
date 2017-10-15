#include <GLES2/gl2.h>

#include "basic_shader.h"
#include "egl_handler.h"
#include "shader_program.h"

int main(int /*argc*/, char ** /*argv*/)
{
    EGLHandler egl_handler;
    egl_handler.init();
    egl_handler.egl_from_dispmanx();

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.0f,
        0.0f, 0.0f, 1.0f,

        0.5f, -0.5f, 0.0f,
        1.0f, 1.0f, 0.0f
     };

     GLubyte indecies[] = {0, 1, 2, 2, 3, 0};

    glViewport(0, 0, egl_handler.screen_width(), egl_handler.screen_height());

    ShaderProgram shader_program;
    shader_program.init(BasicShader::vertex_shader, BasicShader::fragment_shader);
    shader_program.use();

    glClearColor(0.0, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vertices + 3);
    glEnableVertexAttribArray(1);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indecies);

    glFlush();
    eglSwapBuffers(egl_handler.state()->display, egl_handler.state()->surface);
    sleep(2);
    return 0;
}
