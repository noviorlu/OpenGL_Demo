#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#include "Mesh.h"
#include "Material.h"
#include "../Transform.hpp"

namespace GLCore::Utils {
	class Model {
	public:
		std::string m_Name;

		// World Transform
		Transform m_Transform;
		
		std::string m_Directory;
		// Meshes to be drawn
		std::vector<std::shared_ptr<Mesh>> m_Meshes;

		// [TODO]: Resource Pools, not sure how in other projects define unique id
		// for each resource, Should be in a Seperate monosingleton class
		std::unordered_map<std::string, std::shared_ptr<Mesh>> m_MeshPool;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_MaterialPool;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_TexturePool;
		//std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderPool;

		bool m_AutoRotate = false;
		float m_RotationSpeed = 1.0f;
	public:
		Model() {}
		Model(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		~Model() {}
		
		void OnImGuiRender();
		void Draw(Shader& shader);
	};
}