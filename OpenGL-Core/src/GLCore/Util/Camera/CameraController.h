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
		virtual bool OnWindowResized(WindowResizeEvent& e) {
			m_Camera->OnWindowResized(e.GetWidth(), e.GetHeight());
			return false;
		}
		virtual bool OnMousePressed(MouseButtonPressedEvent& e) { return false; }
		virtual bool OnMouseReleased(MouseButtonReleasedEvent& e) { return false; }

	protected:
		void KeyboardMovement(Timestep ts);

	protected:
		Camera* m_Camera;
		glm::vec3 m_Position;
		glm::vec3 m_Target;

		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		
	};
}