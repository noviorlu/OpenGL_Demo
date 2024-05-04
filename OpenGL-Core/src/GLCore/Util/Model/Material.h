#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace GLCore::Utils {
	class Material { 
	public:
		std::vector<std::shared_ptr<Texture>> m_Textures;
		virtual void OnImGuiRender() {}
	};

	class PhongMaterial : public Material {
	public:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;

		PhongMaterial(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
			: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
	
		virtual void OnImGuiRender() override {
			ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
			ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
			ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
			ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
		}
	};

	class PBRMaterial : public Material {};
}