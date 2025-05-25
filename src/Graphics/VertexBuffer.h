#pragma once

#include <glad/gl.h>

namespace RecyclingGame {

    enum class DataUsage {
        STATIC_DRAW = GL_STATIC_DRAW,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        STREAM_DRAW = GL_STREAM_DRAW
    };

    enum class DataType {
        BYTE = GL_BYTE,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        SHORT = GL_SHORT,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        INT = GL_INT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        FLOAT = GL_FLOAT,
        DOUBLE = GL_DOUBLE
    };

    enum class DataElements {
        ONE = 1, TWO = 2, THREE = 3, FOUR = 4
    };

    class VertexBuffer {
    public:

        VertexBuffer() : VertexBuffer(DataUsage::STATIC_DRAW) {}
        explicit VertexBuffer(DataUsage usage);
        ~VertexBuffer();

        void bind();
        
        void setData(const void* data, unsigned int size) { setData(data, size, DataUsage::STATIC_DRAW); }
        void setData(const void* data, unsigned int size, DataUsage usage);

    private:

        unsigned int m_handle;
        DataUsage m_usage;
        
    };
    
}
