#pragma once
#include "Mesh.h"
#include <string>
#include <memory>

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

namespace GLCore::Utils {
	class Shader;

	class Model {
	public:
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		std::string m_Directory;
		std::vector<Texture> m_LoadedTextures;
		
		glm::vec3 m_Position = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 m_Rotation = glm::vec3(0.0, 0.0, 0.0);
		glm::vec3 m_Scale = glm::vec3(1.0, 1.0, 1.0);

		glm::mat4 m_ModelMatrix = glm::mat4(1.0f);

	public:
		Model(std::string const& path);
		Model(std::string const& path, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		~Model();
		
		void SetModifyFlag() { m_modifyFlag = true; }

		void Draw(Shader& shader);
		void LoadModel(const std::string& path);
		
		//void processNode(aiNode* node, const aiScene* scene);
		//std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
		//std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		const glm::mat4& GetModelMatrix();
	
	private:
		void UpdateModelMatrix();
		bool m_modifyFlag = true;
	};
}