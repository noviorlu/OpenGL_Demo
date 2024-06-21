#include "glpch.h"
#include "Material.h"

namespace GLCore::Utils {
	BlinnPhongMaterial::BlinnPhongMaterial(
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

	BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<Material> mat)
	{
		auto pbrMat = std::dynamic_pointer_cast<PBRMaterial>(mat);
		m_Name = pbrMat->m_Name;
		m_Ambient = glm::vec3(1.0f);
		m_Diffuse = glm::vec3(1.0f);
		m_Specular = glm::vec3(1.0f);
		m_Shininess = 1.0f;
		m_BaseColor = pbrMat->m_PBRMetallicRoughness.baseColorFactor;
		m_BaseColorTexture = pbrMat->m_PBRMetallicRoughness.baseColorTexture;
	}

	void BlinnPhongMaterial::Draw(Shader& shader) {
		shader.SetUniform3fv("u_Material.ambient", m_Ambient);
		shader.SetUniform3fv("u_Material.diffuse", m_Diffuse);
		shader.SetUniform3fv("u_Material.specular", m_Specular);
		shader.SetUniform1f("u_Material.shininess", m_Shininess);

		if (m_BaseColorTexture) {
			shader.SetUniform1i("u_Material.baseColorTexture", 0);
			shader.SetUniform1i("u_Material.useBaseColorTexture", 1);
			m_BaseColorTexture->Bind(0);
		}
		else {
			shader.SetUniform4fv("u_Material.baseColor", m_BaseColor);
			shader.SetUniform1i("u_Material.useBaseColorTexture", 0);
		}
	}

	void BlinnPhongMaterial::OnImGuiRender() {
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
		m_DoubleSided(doubleSided) 
	{
		m_Name = name;
		m_PBRMetallicRoughness.baseColorFactor = baseColorFactor;
		if (baseColorTexture) {
			m_PBRMetallicRoughness.baseColorTexture = baseColorTexture;
			m_PBRMetallicRoughness.baseColorFactor = glm::vec4(-1.0f);
		}
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

		// bind base color texture, 1 is the texture unit
		// name here should be match what defined in shader
		if (m_PBRMetallicRoughness.baseColorTexture) {
			shader.SetUniform1i("material.baseColorTexture", 0);
			m_PBRMetallicRoughness.baseColorTexture->Bind(0);
		}
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