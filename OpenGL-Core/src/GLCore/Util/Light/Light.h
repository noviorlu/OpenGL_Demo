#pragma once

#include "../Transform.hpp"
#include "../GLAbstract/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GLCore::Utils {
	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT
	};

	class Light {
	public:
		Light(LightType type, const glm::vec3& position) {
			m_Type = type;
			m_Transform.SetTranslation(position);
		}

		virtual void OnImGuiRender() {
			m_Transform.OnImGuiRender();
		}

		virtual void Draw(Shader& shader, std::string prefix) {
			shader.SetUniform3fv(prefix + ".position", m_Transform.GetTranslation());
		}

	public:
		LightType m_Type;
		Transform m_Transform;
	};

	class BlinnPhongLight : public Light {
	public:
		BlinnPhongLight(LightType type, const glm::vec3& position, const glm::vec3& lightColor, float intensity)
			: Light(type, position), m_LightColor(lightColor), m_Intensity(intensity) { }

		void Draw(Shader& shader, std::string prefix) override {
			Light::Draw(shader, prefix);
			shader.SetUniform3fv(prefix + ".lightColor", m_LightColor);
			shader.SetUniform1f(prefix + ".intensity", m_Intensity);
		}

		void OnImGuiRender() override {
			Light::OnImGuiRender();
			ImGui::ColorEdit3("Light Color", glm::value_ptr(m_LightColor));
			ImGui::SliderFloat("Intensity", &m_Intensity, 1.0f, 1000.0f);
		}
	public:
		glm::vec3 m_LightColor;
		float m_Intensity; // if lux range: 107,527 cd ~ 0.0001 cd, but currently use 1.0f ~ 0.0f
	};
}