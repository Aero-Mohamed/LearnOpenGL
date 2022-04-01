#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(X) if(!(X)) __debugbreak();
#define GlCall(X) GlClearError(); X; ASSERT(GlLogCall(#X, __FILE__, __LINE__))

void GlClearError();

bool GlLogCall(const char* function, const char* file, int line);


class Renderer
{
};

