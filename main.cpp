#include <GLES2/gl2.h>

#include "egl_handler.h"

int main(int argc, char *argv[])
{
    EGLHandler egl_handler;
    egl_handler.init();
    egl_handler.egl_from_dispmanx();
    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    eglSwapBuffers(egl_handler.state()->display, egl_handler.state()->surface);
    sleep(4);
    return 0;
}
