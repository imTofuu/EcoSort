#include "AssetFetcher.h"

#include <tiny_obj_loader.h>

#include "Game.h"
#include <string>

namespace EcoSort {
    
    struct Vertex {
        int positionIndex, normalIndex, uvIndex;

        // A comparison operator is required for unordered containers.
        bool operator==(const Vertex& other) const {
            return positionIndex == other.positionIndex &&
                   normalIndex == other.normalIndex &&
                   uvIndex == other.uvIndex;
        }
    };
    
}

// This hash function is used by the stl in objects like unordered_maps which use hashing
// as keys. It hashes the individual fields in the struct to make a new unique hash.
template<>
struct std::hash<EcoSort::Vertex> {
    size_t operator()(const EcoSort::Vertex& vertex) const noexcept {
        size_t h1 = std::hash<int>()(vertex.positionIndex);
        size_t h2 = std::hash<int>()(vertex.normalIndex);
        size_t h3 = std::hash<int>()(vertex.uvIndex);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

namespace EcoSort {
    std::shared_ptr<Mesh> AssetFetcher::meshFromPath(const char* path) {

        LOGGER.debug("Reading mesh from path: {}", path);

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

        static tinyobj::ObjReader reader;
        static tinyobj::ObjReaderConfig config;
        config.triangulate = true; // Convert non-triangular faces to triangles
        
        bool result = reader.ParseFromFile(path, config);
        
        if (!result) {
            LOGGER.warn("Failed to load mesh from path: {}", path);
            LOGGER.weakAssert(reader.Warning().empty(), "Warning reading mesh: {}", reader.Warning());
            LOGGER.weakAssert(reader.Error().empty(), "Failed to load mesh {}", reader.Error());
            return mesh;
        }

        LOGGER.weakAssert(reader.Warning().empty(), "Warning reading mesh: {}", reader.Warning().c_str());

        // Attribs describe the data, shape describes the mesh structure.
        const tinyobj::attrib_t& attribs = reader.GetAttrib();
        const tinyobj::shape_t& shape = reader.GetShapes()[0]; // TODO: multiple shapes

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        std::vector<float> normals;
        std::vector<float> uvs;

        std::unordered_map<Vertex, unsigned int> uniqueVertices;

        for (auto& index : shape.mesh.indices) {

            // Create a vertex composed of the position, normal, and UV indices as stored in the OBJ, to check if it has
            // already been copied.
            Vertex vertex = {
                index.vertex_index,
                index.normal_index,
                index.texcoord_index
            };
            
            if (auto it = uniqueVertices.find(vertex); it != uniqueVertices.end()) {
                indices.push_back(it->second);
                continue;
            }
            
            // Append the vertex to the buffer
            vertices.push_back(attribs.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attribs.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attribs.vertices[3 * index.vertex_index + 2]);

            // Append the normal to the buffer
            if (index.normal_index >= 0) {
                normals.push_back(attribs.normals[3 * index.normal_index + 0]);
                normals.push_back(attribs.normals[3 * index.normal_index + 1]);
                normals.push_back(attribs.normals[3 * index.normal_index + 2]);
            }

            // Append the UV coordinates to the buffer, or 0.0f if no UVs are present.
            if (index.texcoord_index >= 0) {
                uvs.push_back(attribs.texcoords[2 * index.texcoord_index + 0]);
                uvs.push_back(attribs.texcoords[2 * index.texcoord_index + 1]);
            } else {
                uvs.push_back(0.0f);
                uvs.push_back(0.0f);
            }

            // If the vertex is unique, append indices with the index of this vertex, which is the number of unique
            // vertices already copied, since the data was appended to the back of the vectors.
            indices.push_back(static_cast<unsigned int>(uniqueVertices.size()));
            uniqueVertices[vertex] = indices.back();
        }

        mesh->setVertices(vertices.data(), vertices.size() / 3);
        mesh->setIndices(indices.data(), indices.size());
        mesh->setBuffer(1,
            normals.data(),
            normals.size() * sizeof(float),
            DataType::FLOAT,
            DataElements::THREE);

        mesh->setBuffer(2,
            uvs.data(),
            uvs.size() * sizeof(float),
            DataType::FLOAT,
            DataElements::TWO);

        return mesh;
    }

}
