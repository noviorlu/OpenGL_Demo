#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <json/json.hpp>

#include "Model.h"

using Vertex = GLCore::Utils::Vertex;
using json = nlohmann::json;

namespace GLCore::Utils {
    class glTFLoader {
    public:
        glTFLoader(const char* file, Model* model);
    private:
        // path + name of GLTF file
        const char* m_File = nullptr;
        Model* m_Model = nullptr;
        
        Mesh* currMesh = nullptr;
        Texture* currTexture = nullptr;

        std::vector<unsigned char> m_Data;
        json JSON;

    private:
        std::vector<unsigned char> getData();
        void traverseNode(unsigned int nextNode, Transform* prevTrans);
        void loadMesh(unsigned int indMesh, Transform* curTrans);


        std::vector<float> getFloats(json accessor);
        std::vector<unsigned int> getIndices(json accessor);
        std::vector<std::shared_ptr<Texture>> getTextures();
        std::vector<Vertex> assembleVertices
        (
            std::vector<glm::vec3> positions,
            std::vector<glm::vec3> normals,
            std::vector<glm::vec2> texUVs
        );

        // Helps with the assembly from above by grouping floats
        std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
        std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
        std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
    };
}