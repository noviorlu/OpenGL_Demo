#pragma once
#include "Camera.h"
#include "GLCore/Core/Timestep.h"
#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"

#include <memory>

namespace GLCore::Utils {
	class CameraController
	{
	public:
		CameraController() = default;

		Camera* GetCamera() { return m_Camera; } // non-const version
		const Camera* GetCamera() const { return m_Camera; } // const version

		virtual void OnUpdate(Timestep ts);
		virtual void OnEvent(Event& e);
		virtual void OnImGuiRender();

		virtual bool OnMouseScrolled(MouseScrolledEvent& e) { return false; }
		virtual bool OnWindowResized(WindowResizeEvent& e) { return false; }
		virtual bool OnMousePressed(MouseButtonPressedEvent& e) { return false; }
		virtual bool OnMouseReleased(MouseButtonReleasedEvent& e) { return false; }
	
	protected:
		Camera* m_Camera;
		glm::vec3 m_tempPosition;

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}