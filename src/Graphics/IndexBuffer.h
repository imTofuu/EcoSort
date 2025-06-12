#pragma once

namespace EcoSort {

    class IndexBuffer {
    public:

        IndexBuffer();
        ~IndexBuffer();

        void bind();
        
        void setData(const unsigned int* indices, unsigned int count);
        unsigned int getNumIndices() { return m_count; }

    private:

        unsigned int m_handle;
        unsigned int m_count;
        
    };
    
}