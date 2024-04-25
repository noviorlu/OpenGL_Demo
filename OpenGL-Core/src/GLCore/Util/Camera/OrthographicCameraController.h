#pragma once

#include "OrthographicCamera.h"
#include "CameraController.h"
#include "GLCore/Core/Timestep.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"

namespace GLCore::Utils {

	class OrthographicCameraController : public CameraController
	{
	public:
		static constexpr float DEFAULT_ASPECTRATIO = 16/9;

		OrthographicCameraController(float aspectRatio = DEFAULT_ASPECTRATIO);

		void OnUpdate(Timestep ts) override;
		//void OnImGuiRender() override;

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e) override;
		bool OnWindowResized(WindowResizeEvent& e) override;
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}