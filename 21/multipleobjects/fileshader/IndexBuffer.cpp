
#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
    m_Count(count)
{
    CALLGL(glGenBuffers(1, &m_RenderID));
    CALLGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
    CALLGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        count * sizeof(unsigned int), //_countof(positions) * sizeof(float),  // 6 * 2 * sizeof(float)
        data,
        GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer()
{
    CALLGL(glDeleteBuffers(1, &m_RenderID));
}

void IndexBuffer::Bind() const
{
    CALLGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}
void IndexBuffer::Unbind() const
{
    CALLGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}