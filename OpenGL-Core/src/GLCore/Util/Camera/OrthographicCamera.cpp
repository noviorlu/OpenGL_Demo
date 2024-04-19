#include "glpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace GLCore::Utils {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: Camera()
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateVPMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateVPMatrix();
	}
}