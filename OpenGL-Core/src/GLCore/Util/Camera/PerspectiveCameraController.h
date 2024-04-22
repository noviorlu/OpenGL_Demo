#pragma once

#include "PerspectiveCamera.h"
#include "CameraController.h"
#include "GLCore/Core/Timestep.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"

namespace GLCore::Utils {

	class PerspectiveCameraController : public CameraController
	{
	public:
		PerspectiveCameraController();
		~PerspectiveCameraController();

		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);

	private:

		float m_MouseSensitivity = 0.1f;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		
		bool m_LeftMousePressed = false;
		bool RightMousePressed = false;
		glm::vec2 m_InitialMousePosition;
		float m_LastX, m_LastY;
	};

}