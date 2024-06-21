#pragma once

#include "Light.h"
#include "../GLAbstract/Shader.h"
#include "../Model/Mesh.h"
#include <memory>

namespace GLCore::Utils {
	class LightGroup {
	public:
		LightGroup() {
			m_Icon = Cube();
		}
		~LightGroup() {
			m_Lights.clear();
		}
		void AddLight(std::shared_ptr<Light> light) { m_Lights.push_back(light); }

		virtual void OnImGuiRender() {
			ImGui::Begin("LightGroup");
			for (auto& light : m_Lights) {
				ImGui::Text("Light Type: %d", light->m_Type);
				light->OnImGuiRender();
			}
			ImGui::End();
		}
		void DrawIcon(Shader& shader) {
			for (auto& light : m_Lights) {
				shader.SetUniformMat4f("u_Model", light->m_Transform.GetModelMatrix());
				m_Icon.Draw(shader);
			}
		}

		virtual void Draw(Shader& shader) {
			int directionalLightIndex = 0;
			int pointLightIndex = 0;
			for (auto& light : m_Lights) {
				if (light->m_Type == LightType::DIRECTIONAL) {
					light->Draw(shader, "u_DirectionalLights[" + std::to_string(directionalLightIndex) + "]");
					directionalLightIndex++;
				}
				else if (light->m_Type == LightType::POINT) {
					light->Draw(shader, "u_PointLights[" + std::to_string(pointLightIndex) + "]");
					pointLightIndex++;
				}
			}
			shader.SetUniform1i("u_DirectionalLightCount", directionalLightIndex+1);
			shader.SetUniform1i("u_PointLightCount", pointLightIndex+1);
		}
	private:
		std::vector<std::shared_ptr<Light>> m_Lights;
		Cube m_Icon;
	};

	class BlinnPhongLightGroup : public LightGroup {
	public:
		BlinnPhongLightGroup(glm::vec3 ambientLightColor, float ambientIntensity)
			: m_AmbientLightColor(ambientLightColor), m_AmbientIntensity(ambientIntensity) { }

		void OnImGuiRender() override {
			LightGroup::OnImGuiRender();
			ImGui::Begin("Ambient Light");
			ImGui::ColorEdit3("Ambient Light Color", glm::value_ptr(m_AmbientLightColor));
			ImGui::SliderFloat("Ambient Intensity", &m_AmbientIntensity, 0.0f, 1.0f);
			ImGui::End();
		}

		void Draw(Shader& shader) override {
			LightGroup::Draw(shader);
			shader.SetUniform3fv("u_AmbientLight", m_AmbientLightColor);
			shader.SetUniform1f("u_AmbientIntensity", m_AmbientIntensity);
		
		}
	private:
		glm::vec3 m_AmbientLightColor;
		float m_AmbientIntensity;
	};
}