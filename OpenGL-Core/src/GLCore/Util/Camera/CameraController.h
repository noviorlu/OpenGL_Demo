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
		static constexpr float CC_YAW = -90.0f;
		static constexpr float CC_PITCH = 0.0f;
		static constexpr float CC_SPEED = 2.5f;
		static constexpr float CC_SENSITIVITY = 0.1f;

		CameraController(
			float yaw = CC_YAW,
			float pitch = CC_PITCH,
			float speed = CC_SPEED,
			float sensitivity = CC_SENSITIVITY
		)
			: m_Yaw(yaw), m_Pitch(pitch), m_CameraTranslationSpeed(speed), m_CameraRotationSpeed(sensitivity) {}

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
		glm::vec3 m_Position = glm::vec3(0, 0, 0);
		glm::vec3 m_Target = glm::vec3(0,0,0);
		bool isFreelook = true;
		float m_Radius = 10.0f;

		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		
	};
}