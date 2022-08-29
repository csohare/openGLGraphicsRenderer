#pragma once

#include <GL\glew.h>

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x))  __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
private:

public:
    Renderer();
    ~Renderer();

    void Draw(const VertexArray& va, const Buffer& indexBuffer, const Shader& shader) const;
};
