#include <iostream>
#include "Renderer.h"

void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error]: Code (" << error << ")\n"
            << "Function: " << function << "\n"
            << "File: " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}