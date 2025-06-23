#pragma once

#include <memory>
#include <vector>

#include "IndexBuffer.h"
#include "VertexArray.h"

namespace EcoSort {
    class Mesh {
    public:

        void setVertices(const float *data, unsigned int vertices);
        void setVertices(std::shared_ptr<VertexBuffer> &vbo);
        
        void setIndices(const unsigned int *indices, unsigned int count);
        void setIndices(std::shared_ptr<IndexBuffer> &ibo);

        void setBuffer(unsigned int index, const void *data, unsigned int size, DataType type, DataElements elements);
        void setBuffer(unsigned int index, std::shared_ptr<VertexBuffer> &vbo, DataType type, DataElements elements);

        void draw();

    private:
        
        std::shared_ptr<VertexArray> m_vao = std::make_shared<VertexArray>();
        std::shared_ptr<IndexBuffer> m_ibo;

        std::vector<std::shared_ptr<VertexBuffer>> m_buffers;

        unsigned int m_indexCount = 0;
    };
}
