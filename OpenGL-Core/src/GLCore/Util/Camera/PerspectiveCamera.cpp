#include "glpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <iostream>

namespace GLCore::Utils {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float n, float f)
		: Camera(), m_Fov(fov), m_Near(n), m_Far(f)
	{
		// executed after Camera Base query screen size
		m_Aspect = (m_viewPortWidth * m_fullScreenWidth) / (m_viewPortHeight * m_fullScreenHeight);
		RecalculateProjectionMatrix();
	}

	void PerspectiveCamera::OffsetFov(float offset) {
		if (m_Fov >= 1.0f && m_Fov <= 160.0f) {
			m_Fov += offset;
			RecalculateProjectionMatrix();
		}
	}

	void PerspectiveCamera::OnWindowResized(uint32_t width, uint32_t height) {
		Camera::OnWindowResized(width, height);
		m_Aspect = (m_viewPortWidth * m_fullScreenWidth) / (m_viewPortHeight * m_fullScreenHeight);
		RecalculateProjectionMatrix();
	}

	void PerspectiveCamera::RecalculateProjectionMatrix() {
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
		RecalculateVPMatrix();
	}

	void PerspectiveCamera::OnImGuiRender() {
		Camera::OnImGuiRender();
		ImGui::Text("Perspective Camera");
		if(ImGui::SliderFloat("Field of View", &m_Fov, 0.0f, 180.0f))
			RecalculateProjectionMatrix();
		if(ImGui::SliderFloat("Near Plane n", &m_Near, 0.01f, 100.0f))
			RecalculateProjectionMatrix();
		if(ImGui::SliderFloat("Far Plane f", &m_Far, 0.01f, 100.0f))
			RecalculateProjectionMatrix();
	}
}