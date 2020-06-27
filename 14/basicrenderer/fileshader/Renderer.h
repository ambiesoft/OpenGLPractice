#pragma once

#include <string>
#include <GL/glew.h>

#define ASSERT_GL(x) do { if(!(x)) { __debugbreak(); } } while(false)
#define CALLGL(x) do {                                      \
    ClearGLError();                                         \
    x;                                                      \
    ASSERT_GL(LogGLCall(#x, __FILE__, __LINE__));           \
} while(false)

void ClearGLError();
std::string GetGLErrorString(GLenum error);
bool LogGLCall(const char* funcname, const char* filename, int line);
