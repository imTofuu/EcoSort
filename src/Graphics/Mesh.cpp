#include "Mesh.h"

#include "Game.h"

namespace EcoSort {

    void Mesh::setVertices(std::shared_ptr<VertexBuffer>& vbo) {
        // Always use index 0 for positions for simplicity
        m_vao->setBuffer(0, *vbo, DataType::FLOAT, DataElements::THREE);
        // Keep an owning reference of the vbo to ensure the data is kept alive until it is not necessary any more
        m_buffers.emplace_back(vbo);
    }

    void Mesh::setIndices(std::shared_ptr<IndexBuffer>& ibo) {
        m_ibo = ibo;
        m_indexCount = ibo ? ibo->getNumIndices() : 0;
    }

    void Mesh::setVertices(const float* data, unsigned int vertices) {
        m_vao->bind();
        auto vbo = std::make_shared<VertexBuffer>();
        vbo->bind();
        vbo->setData(data, vertices * sizeof(float) * 3);
        setVertices(vbo);
    }

    void Mesh::setIndices(const unsigned int* indices, unsigned int count) {
        auto ibo = std::make_shared<IndexBuffer>();
        ibo->bind();
        ibo->setData(indices, count);
        setIndices(ibo);
    }

    void Mesh::setBuffer(unsigned int index, std::shared_ptr<VertexBuffer>& vbo, DataType type, DataElements elements) {
        // Ensure the index is not 0, which is reserved for positions, unless the buffer is empty since drawing is not
        // guaranteed to be done with 3-dimensional coordinates.
        if (!index && !m_buffers.empty()) {
            LOGGER.warn("An index of 0 is reserved for positions. Buffer placed at back (index {}) instead.",
                m_buffers.size());
            index = m_buffers.size();
        }
        m_vao->setBuffer(index, *vbo, type, elements);
        // Keep an owning reference of the vbo to ensure the data is kept alive until it is not necessary any more
        m_buffers.emplace_back(vbo);
    }

    void Mesh::setBuffer(unsigned int index, const void* data, unsigned int size, DataType type, DataElements elements) {
        std::shared_ptr<VertexBuffer> vbo = std::make_shared<VertexBuffer>();
        vbo->bind();
        vbo->setData(data, size, DataUsage::STATIC_DRAW);
        setBuffer(index, vbo, type, elements);
    }

    void Mesh::draw() {
        if (!m_ibo) {
            LOGGER.warn("Mesh has no indices");
            return;
        }

        m_vao->bind();
        m_ibo->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLint>(m_indexCount), GL_UNSIGNED_INT, nullptr);
    }
    
}
