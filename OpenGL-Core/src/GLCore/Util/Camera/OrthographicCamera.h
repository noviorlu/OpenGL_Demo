#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
namespace GLCore::Utils {
	

	class OrthographicCamera : public Camera
	{
		static constexpr float DEFAULT_LEFT = -1.0f;
		static constexpr float DEFAULT_RIGHT = 1.0f;
		static constexpr float DEFAULT_BOTTOM = -1.0f;
		static constexpr float DEFAULT_TOP = 1.0f;
		static constexpr float DEFAULT_ZNEAR = -1.0f;
		static constexpr float DEFAULT_ZFAR = 1.0f;
	public:
		OrthographicCamera(float left = DEFAULT_LEFT, float right = DEFAULT_RIGHT, float bottom = DEFAULT_BOTTOM, float top = DEFAULT_TOP);

		void SetProjection(float left, float right, float bottom, float top);

	private:
		void RecalculateProjectionMatrix() override;
	private:
		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;
		float m_zNear = -1.0f;
		float m_zFar = 1.0f;

	};

}
