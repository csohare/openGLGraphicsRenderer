#include "Renderer.h"
#include <iostream>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)    
{

    if (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR] " << error << " " << function << " "
            << file << " " << line << std::endl;
        std::cin.get();
        return false;
	}
    return true;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(const VertexArray& va, const Buffer& IndexBuffer, const Shader& shader) const
{
    shader.Bind(); 
    va.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, IndexBuffer.getCount(), GL_UNSIGNED_INT, 0));
}
