#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "../GLAbstract/Shader.h"
#include "../GLAbstract/Texture.h"

namespace GLCore::Utils {
	class Material { 
	public:
		enum MatDataType {
			BLINNPHONG = 0,
			PBR
		};

		std::string m_Name;
		int m_SeralizeID;
	public:
		virtual void OnImGuiRender() = 0;
		virtual void Draw(Shader& shader) = 0;
		virtual int Seralize(std::vector<float>& Matdata) = 0;
	};

	class BlinnPhongMaterial : public Material {
	public:
		static constexpr glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.2f);
		static constexpr glm::vec3 DEFAULT_DIFFUSE = glm::vec3(0.8f);
		static constexpr glm::vec3 DEFAULT_SPECULAR = glm::vec3(1.0f);
		static constexpr float DEFAULT_SHININESS = 32.0f;

		glm::vec3 m_Ambient; // ka
		glm::vec3 m_Diffuse; // kd
		glm::vec3 m_Specular; // ks
		float m_Shininess; // p

		glm::vec4 m_BaseColor;
		std::shared_ptr<Texture> m_BaseColorTexture;

	public:
		BlinnPhongMaterial(
			const glm::vec3& ambient = DEFAULT_AMBIENT,
			const glm::vec3& diffuse = DEFAULT_DIFFUSE,
			const glm::vec3& specular = DEFAULT_SPECULAR,
			float shininess = DEFAULT_SHININESS);
	
		BlinnPhongMaterial(std::shared_ptr<Material> mat);
		void Draw(Shader& shader) override;
		void OnImGuiRender() override;
		int Seralize(std::vector<float>& Matdata) override;
	};

	class PBRMaterial : public Material {
	public:
		glm::vec3 m_EmissiveFactor;
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
		int Seralize(std::vector<float>& Matdata) override;
	};
}