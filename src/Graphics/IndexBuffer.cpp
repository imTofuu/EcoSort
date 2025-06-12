#include "IndexBuffer.h"

#include <glad/gl.h>

namespace EcoSort {

    IndexBuffer::IndexBuffer() : m_handle(0), m_count(0) {
        glGenBuffers(1, &m_handle);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_handle);
    }
    
    void IndexBuffer::bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    }

    void IndexBuffer::setData(const unsigned int* indices, unsigned int count) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        m_count = count;
    }
    
}
