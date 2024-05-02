#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#include "Mesh.h"
#include "../Transform.hpp"

namespace GLCore::Utils {
	class Shader;

	class Model {
	public:
		// World Transform
		Transform m_Transform;
		
		std::string m_Directory;
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_LoadedTextures;
	
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