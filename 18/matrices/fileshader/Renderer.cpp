#include <iostream>
#include "Renderer.h"

void ClearGLError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}
std::string GetGLErrorString(GLenum error)
{
    return (const char*)gluErrorString(error);
}
bool LogGLCall(const char* funcname, const char* filename, int line)
{
    while (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL error]" << "(" << error << "[" << GetGLErrorString(error) << "]" <<
            "):" <<
            funcname << " " << filename << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    CALLGL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Bind
    shader.Bind();

    va.Bind();
    ib.Bind();

    CALLGL(glDrawElements(GL_TRIANGLES,
        ib.GetCount(),
        GL_UNSIGNED_INT,
        nullptr));
}