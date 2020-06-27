
#include "Renderer.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    CALLGL(glGenBuffers(1, &m_RenderID));
    CALLGL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
    CALLGL(glBufferData(GL_ARRAY_BUFFER,
        size, //_countof(positions) * sizeof(float),  // 6 * 2 * sizeof(float)
        data,
        GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer()
{
    CALLGL(glDeleteBuffers(1, &m_RenderID));
}

void VertexBuffer::Bind()
{
    CALLGL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}
void VertexBuffer::Unbind()
{
    CALLGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}