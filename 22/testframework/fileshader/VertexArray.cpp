
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray() 
{
    CALLGL(glGenVertexArrays(1, &m_RendererID));
    CALLGL(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
    CALLGL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
	vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];

        // Enable attribute0
        CALLGL(glEnableVertexAttribArray(i)); // first attribute

        // Layout the buffer
        // set attribute0 , decides the composition of memory(positions)
        CALLGL(glVertexAttribPointer(
            i, // first attribute
            element.count, // count of point in a vertex(position)
            element.type,
            element.normalized, // not normalize
            layout.GetStride(), // sizeof(float) * 2, // stride, byte size of 2 vertexes
            (const void*)offset  // offset
        ));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    CALLGL(glBindVertexArray(m_RendererID));
}
void VertexArray::Unbind() const
{
    CALLGL(glBindVertexArray(0));
}