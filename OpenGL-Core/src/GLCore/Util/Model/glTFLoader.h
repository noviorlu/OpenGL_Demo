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
        std::string m_File;
        std::string m_FileDir;

        // Data from the file
        std::vector<unsigned char> m_Data;
        json JSON;

        // Model to be loaded into
        Model* m_Model = nullptr;

    private:
        std::vector<unsigned char> getData();
        void traverseNode(unsigned int nextNode, Transform* prevTrans);
        void loadMesh(unsigned int indMesh, Transform* curTrans);

        std::vector<float> getFloats(json accessor);
        std::vector<unsigned int> getIndices(json accessor);

        std::shared_ptr<Texture> getTextures(unsigned int indTex);
        std::shared_ptr<Material> getMaterial(unsigned int indMat);
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