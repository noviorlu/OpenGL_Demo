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
		Light(LightType type, const glm::vec3& position, const glm::vec3& intensity) {
			m_Type = type;
			m_Transform.SetTranslation(position);
			m_Intensity = intensity;
		}

		virtual void OnImGuiRender() {
			m_Transform.OnImGuiRender();
			ImGui::ColorEdit3("Intensity", glm::value_ptr(m_Intensity));
		}

		virtual void Draw(Shader& shader, std::string prefix) {
			shader.SetUniform3fv(prefix + ".position", m_Transform.GetTranslation());
			shader.SetUniform3fv(prefix + ".intensity", m_Intensity);
		}

	public:
		LightType m_Type;
		Transform m_Transform;
		glm::vec3 m_Intensity; // if lux range: 107,527 cd ~ 0.0001 cd, but currently use 1.0f ~ 0.0f
	};

	class BlinnPhongLight : public Light {
	public:
		BlinnPhongLight(LightType type, const glm::vec3& position, const glm::vec3& intensity, const glm::vec3& ambientIntensity)
			: Light(type, position, intensity), m_AmbientIntensity(ambientIntensity) { }

		void Draw(Shader& shader, std::string prefix) override {
			Light::Draw(shader, prefix);
			shader.SetUniform3fv(prefix + ".ambientIntensity", m_AmbientIntensity);
		}

	public:
		glm::vec3 m_AmbientIntensity;
	};
}