#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "../GLAbstract/Shader.h"
#include "../GLAbstract/Texture.h"

namespace GLCore::Utils {
	class Material { 
	public:
		std::string m_Name;

	public:
		virtual void OnImGuiRender() = 0;
		virtual void Draw(Shader& shader) = 0;
	};

	class PhongMaterial : public Material {
	public:
		static constexpr glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.2f);
		static constexpr glm::vec3 DEFAULT_DIFFUSE = glm::vec3(0.8f);
		static constexpr glm::vec3 DEFAULT_SPECULAR = glm::vec3(1.0f);
		static constexpr float DEFAULT_SHININESS = 32.0f;

		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Shininess;

	public:
		PhongMaterial(
			const glm::vec3& ambient = DEFAULT_AMBIENT,
			const glm::vec3& diffuse = DEFAULT_DIFFUSE,
			const glm::vec3& specular = DEFAULT_SPECULAR,
			float shininess = DEFAULT_SHININESS);
	
		void Draw(Shader& shader) override;
		void OnImGuiRender() override;
	};

	class PBRMaterial : public Material {
	public:
		glm::vec3 m_EmissiveFactor;
		std::string m_Name;
		bool m_DoubleSided;

		struct PBRMetallicRoughness {
			glm::vec4 baseColorFactor;
			std::shared_ptr<Texture> baseColorTexture;
			float metallicFactor;
			float roughnessFactor;
		} m_PBRMetallicRoughness;

	public:
		PBRMaterial(
			const glm::vec3& emissiveFactor,
			const std::string& name,
			bool doubleSided,
			const glm::vec4& baseColorFactor,
			const std::shared_ptr<Texture>& baseColorTexture,
			float metallicFactor,
			float roughnessFactor
		);

		void Draw(Shader& shader) override;
		void OnImGuiRender() override;
	};
}