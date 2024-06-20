#pragma once

#include <glm/glm.hpp>
#include "GLCore/Util/GLAbstract/Shader.h"
#include "Camera.h"
namespace GLCore::Utils {

	class PerspectiveCamera : public Camera
	{
	public:
		static constexpr float DEFAULT_FOV = 45.0f;
		static constexpr float DEFAULT_ASPECT = 16.0f / 9.0f;
		static constexpr float DEFAULT_ZNEAR = 0.1f;
		static constexpr float DEFAULT_ZFAR = 100.0f;

		PerspectiveCamera(float fov = DEFAULT_FOV, float aspect = DEFAULT_ASPECT, float n = DEFAULT_ZNEAR, float f = DEFAULT_ZFAR);
		PerspectiveCamera(const glm::vec3 position);

		void SetFov(float fov)	{ m_Fov = fov;	RecalculateProjectionMatrix(); }
		void SetNear(float n)	{ m_Near = n;	RecalculateProjectionMatrix(); }
		void SetFar(float f)	{ m_Far = f;	RecalculateProjectionMatrix(); }

		const float GetFov() const { return m_Fov; }
		const float GetAspect() const { return m_Aspect; }
		const float GetNear() const { return m_Near; }
		const float GetFar() const { return m_Far; }

		void OffsetFov(float offset);
		void OnWindowResized(uint32_t width, uint32_t height) override;
		void Draw(Shader& shader) override;
	protected:
		void RecalculateProjectionMatrix() override;
		void OnImGuiRender() override;
	protected:
		float m_Fov;
		float m_Aspect;
		float m_Near;
		float m_Far;
	};
}
