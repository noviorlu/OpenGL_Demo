#pragma once

#include "Light.h"
#include "../GLAbstract/Shader.h"
#include "../Model/Mesh.h"
#include <memory>

namespace GLCore::Utils {
	class LightGroup {
	public:
		LightGroup() {}
		~LightGroup() {
			m_Lights.clear();
		}
		void AddLight(std::shared_ptr<Light> light) { m_Lights.push_back(light); }

		void OnImGuiRender() {
			for (auto& light : m_Lights) {
				light->OnImGuiRender();
			}
		}
		void DrawIcon(Shader& shader) {
			for (auto& light : m_Lights) {
				shader.SetUniformMat4f("u_Model", light->m_Transform.GetModelMatrix());
				m_Icon.Draw(shader);
			}
		}

		void Draw(Shader& shader) {
			int directionalLightIndex = 0;
			int pointLightIndex = 0;
			for (auto& light : m_Lights) {
				if (light->m_Type == LightType::DIRECTIONAL) {
					light->Draw(shader, "u_DirectionalLight[" + std::to_string(directionalLightIndex) + "]");
					directionalLightIndex++;
				}
				else if (light->m_Type == LightType::POINT) {
					light->Draw(shader, "u_PointLight[" + std::to_string(pointLightIndex) + "]");
					pointLightIndex++;
				}
			}
			shader.SetUniform1i("u_DirectionalLightCount", directionalLightIndex);
			shader.SetUniform1i("u_PointLightCount", pointLightIndex);
		}
	private:
		std::vector<std::shared_ptr<Light>> m_Lights;
		Quad m_Icon;
	};
}