#include "VertexBuffer.h"

namespace RecyclingGame {

    VertexBuffer::VertexBuffer(DataUsage usage) : m_handle(0), m_usage(usage) {
        glGenBuffers(1, &m_handle);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_handle);
    }

    void VertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    }

    void VertexBuffer::setData(const void* data, unsigned int size, DataUsage usage) {
        glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
    }
    
}
