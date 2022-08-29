#include "Renderer.h"
#include "Buffer.h"

Buffer::Buffer(const void* data, unsigned int count, unsigned int typeSize, unsigned int target)
    :m_BufferTarget(target), m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(m_BufferTarget, m_RendererID));
    GLCall(glBufferData(m_BufferTarget, count * typeSize, data, GL_STATIC_DRAW));
}

Buffer::~Buffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID))
}

void Buffer::Bind() const
{
    GLCall(glBindBuffer(m_BufferTarget, m_RendererID));
}

void Buffer::Unbind() const
{
    GLCall(glBindBuffer(m_BufferTarget, 0));
}

inline unsigned int Buffer::getCount() const
{
    return m_Count;
}
