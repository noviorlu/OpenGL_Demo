#include "glpch.h"
#include "Material.h"

namespace GLCore::Utils {
	/*void Material::Draw(Shader& shader) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		// bind appropriate textures
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->Bind(i);

			std::string name;
			// name here should be match what defined in shader
			switch (m_Textures[i]->m_Type) {
			case Texture::TextureType::DIFFUSE:
				name = "texture_diffuse" + std::to_string(diffuseNr++);
				break;
			case Texture::TextureType::SPECULAR:
				name = "texture_specular" + std::to_string(specularNr++);
				break;
			case Texture::TextureType::NORMAL:
				name = "texture_normal" + std::to_string(normalNr++);
				break;
			case Texture::TextureType::HEIGHT:
				name = "texture_height" + std::to_string(heightNr++);
				break;
			}

			shader.SetUniform1i(name.c_str(), i);
		}
	}*/

	PhongMaterial::PhongMaterial(
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular,
		float shininess
	)
		: m_Ambient(ambient), 
		m_Diffuse(diffuse), 
		m_Specular(specular), 
		m_Shininess(shininess) 
	{ }

	void PhongMaterial::Draw(Shader& shader) {
		shader.SetUniform3fv("material.ambient", m_Ambient);
		shader.SetUniform3fv("material.diffuse", m_Diffuse);
		shader.SetUniform3fv("material.specular", m_Specular);
		shader.SetUniform1f("material.shininess", m_Shininess);
	}

	void PhongMaterial::OnImGuiRender() {
		ImGui::ColorEdit3("Ambient", glm::value_ptr(m_Ambient));
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(m_Diffuse));
		ImGui::ColorEdit3("Specular", glm::value_ptr(m_Specular));
		ImGui::SliderFloat("Shininess", &m_Shininess, 0.0f, 256.0f);
	}
	PBRMaterial::PBRMaterial(
		const glm::vec3& emissiveFactor, 
		const std::string& name, 
		bool doubleSided, 
		const glm::vec4& baseColorFactor, 
		const std::shared_ptr<Texture>& baseColorTexture, 
		float metallicFactor, 
		float roughnessFactor
	)
		: m_EmissiveFactor(emissiveFactor),
		m_Name(name),
		m_DoubleSided(doubleSided) 
	{
		m_PBRMetallicRoughness.baseColorFactor = baseColorFactor;
		m_PBRMetallicRoughness.baseColorTexture = baseColorTexture;
		m_PBRMetallicRoughness.metallicFactor = metallicFactor;
		m_PBRMetallicRoughness.roughnessFactor = roughnessFactor;
	}
	void PBRMaterial::Draw(Shader& shader)
	{
		shader.SetUniform3fv("material.emissiveFactor", m_EmissiveFactor);
		shader.SetUniform1i("material.doubleSided", m_DoubleSided);
		shader.SetUniform4fv("material.baseColorFactor", m_PBRMetallicRoughness.baseColorFactor);
		shader.SetUniform1f("material.metallicFactor", m_PBRMetallicRoughness.metallicFactor);
		shader.SetUniform1f("material.roughnessFactor", m_PBRMetallicRoughness.roughnessFactor);

		if (m_PBRMetallicRoughness.baseColorTexture)
			m_PBRMetallicRoughness.baseColorTexture->Bind(1);
	}

	void PBRMaterial::OnImGuiRender()
	{
		ImGui::ColorEdit3("Emissive Factor", glm::value_ptr(m_EmissiveFactor));
		ImGui::Text("Name: %s", m_Name.c_str());
		ImGui::Checkbox("Double Sided", &m_DoubleSided);
		ImGui::ColorEdit4("Base Color Factor", glm::value_ptr(m_PBRMetallicRoughness.baseColorFactor));
		ImGui::SliderFloat("Metallic Factor", &m_PBRMetallicRoughness.metallicFactor, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness Factor", &m_PBRMetallicRoughness.roughnessFactor, 0.0f, 1.0f);
	}
}