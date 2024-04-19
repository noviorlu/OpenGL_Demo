#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
namespace GLCore::Utils {

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		float GetRotation() const { return m_Yaw; }
		void SetRotation(float rotation) { m_Yaw = rotation; RecalculateViewMatrix(); }
	};

}
