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

void Renderer::clear(float r, float g, float b, float a) const
{
    // Set Screen Color
    GlCall(glClearColor(r, g, b, a));
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    /* Binds whatever required to Draw */
    shader.bind(); //bind shader program 
    va.bind(); // bind vertex array
    ib.bind(); // bind index buffer

    GlCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
