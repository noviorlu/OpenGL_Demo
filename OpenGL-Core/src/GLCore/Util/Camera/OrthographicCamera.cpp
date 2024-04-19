#include "glpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace GLCore::Utils {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: Camera()
	{
		SetProjection(left, right, bottom, top);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_zNear, m_zFar);
		RecalculateVPMatrix();
	}

	void OrthographicCamera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_zNear, m_zFar);
		RecalculateVPMatrix();
	}
}