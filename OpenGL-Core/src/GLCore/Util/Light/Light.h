#pragma once

#include "../Transform.hpp"

namespace GLCore::Utils {
	class Light {
		Transform m_Transform;
		float m_Intensity;
	};

	class DirectionalLight : public Light {
		glm::vec3 m_Direction;
	};

	class PointLight : public Light {
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		float m_Constant;
		float m_Linear;
		float m_Quadratic;
	};

	class PhongLight : public Light {
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};
}