#include <GLES2/gl2.h>

#include "basic_shader.h"
#include "egl_handler.h"
#include "shader_program.h"
#include "tga_loader.h"

GLuint CreateSimpleTexture2D(const tga_file_t& texture)
{
    // Texture object handle
    GLuint textureId;
    
    uint8_t *pixels = texture.imageData;
    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Generate a texture object
    glGenTextures(1, &textureId);
    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureId);
    // Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        texture.imageWidth, texture.imageHeight,
                 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return textureId;
}

int main(int /*argc*/, char ** /*argv*/)
{
    EGLHandler egl_handler;
    egl_handler.init();
    egl_handler.egl_from_dispmanx();

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, 0.0f,

        -1.0f, 1.0f, 0.0f,
        0.0f, 1.0f,

        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, -1.0f, 0.0f,
        1.0f, 0.0f};

    GLubyte indecies[] = {0, 1, 2, 2, 3, 0};

    glViewport(0, 0, egl_handler.screen_width(), egl_handler.screen_height());

    ShaderProgram shader_program;
    shader_program.init(BasicShader::vertex_shader, BasicShader::fragment_shader);
    shader_program.use();

    tga_file_t texture_tga;
    LoadTGAFile("corsairs.tga", &texture_tga);

    glClearColor(0.0, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Get the sampler location
    GLint texture_location = glGetUniformLocation (shader_program.shader_id(), "u_texture" );
    // Load the texture
    GLuint texture_id = CreateSimpleTexture2D(texture_tga);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices + 3);
    glEnableVertexAttribArray(1);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    // Set the sampler texture unit to 0
    glUniform1i(texture_location, 0 );


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indecies);

    glFlush();
    eglSwapBuffers(egl_handler.state()->display, egl_handler.state()->surface);
    sleep(5);
    return 0;
}
