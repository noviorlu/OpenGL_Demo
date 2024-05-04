#pragma once

#include "../Transform.hpp"

namespace GLCore::Utils {
	class Light {
		Transform m_Transform;
		float m_Intensity;
	};

	class PhongLight : public Light {
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};
}