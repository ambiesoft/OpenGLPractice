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
