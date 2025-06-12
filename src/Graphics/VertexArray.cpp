#include "VertexArray.h"

namespace EcoSort {

    VertexArray::VertexArray() : m_handle(0) {
        glGenVertexArrays(1, &m_handle);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_handle);
    }

    void VertexArray::bind() {
        glBindVertexArray(m_handle);
    }

    // Set an attribute of tightly packed, self normalised data for the vertex array at index and enable it
    void VertexArray::setBuffer(unsigned int index, VertexBuffer& vbo, DataType type, DataElements elements) {
        vbo.bind();
        glVertexAttribPointer(index,
            static_cast<GLint>(elements),
            static_cast<GLenum>(type),
            GL_FALSE,
            0,
            nullptr);
        glEnableVertexAttribArray(index);
    }
    
}