#include "glpch.h"
#include "PerspectiveCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include <imgui.h>
namespace GLCore::Utils {

	PerspectiveCameraController::PerspectiveCameraController(){
		m_Camera = new PerspectiveCamera();
	}

	PerspectiveCameraController::~PerspectiveCameraController() {
		delete m_Camera;
	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		MouseMovement(ts);
		KeyboardMovement(ts);
	}

	void PerspectiveCameraController::OnImGuiRender()
	{
		CameraController::OnImGuiRender();
		ImGui::Text("Perspective Camera Controller");
		ImGui::SliderFloat("Camera Translation Speed", &m_CameraTranslationSpeed, 0.1f, 10.0f);
		ImGui::SliderFloat("Mouse Sensitivity", &m_MouseSensitivity, 0.1f, 10.0f);
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		((PerspectiveCamera*)m_Camera)->OffsetFov(-e.GetYOffset());
		return false;
	}

	bool PerspectiveCameraController::OnMousePressed(MouseButtonPressedEvent& e)
	{
		switch (e.GetMouseButton()) {
		case 0:
			m_LeftMousePressed = true;
			m_LastX = Input::GetMouseX();
			m_LastY = Input::GetMouseY();
			break;
		case 1:
			RightMousePressed = true;
			break;
		}
		return false;
	}

	bool PerspectiveCameraController::OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		switch (e.GetMouseButton()) {
		case 0:
			m_LeftMousePressed = false;
			m_LastX = -1;
			m_LastY = -1;
			break;
		case 1:
			RightMousePressed = false;
			break;
		}
		return false;
	}

	void PerspectiveCameraController::MouseMovement(Timestep ts) {
		if (m_LeftMousePressed)
		{
			auto [x, y] = Input::GetMousePosition();
			float xoffset = x - m_LastX;
			float yoffset = m_LastY - y;
			m_LastX = x;
			m_LastY = y;

			xoffset *= m_MouseSensitivity;
			yoffset *= m_MouseSensitivity;

			OffsetYawPitch(xoffset, yoffset);
		}
	}

	void PerspectiveCameraController::OffsetYawPitch(const float& offsetY, const float& offsetP) {
		m_Yaw += offsetY;
		m_Pitch += offsetP;

		if (m_Yaw > 360.0f) m_Yaw = 0.0f;
		else if (m_Yaw < 0.0f) m_Yaw = 360.0f;

		if (m_Pitch < -180.0f) m_Pitch = 180.0f;
		else if (m_Pitch > 180.0f) m_Pitch = -180.0f;

		if (m_Pitch > 90.0f || m_Pitch < -90.0f)
			m_Camera->SetWorldUp(false);
		else
			m_Camera->SetWorldUp(true);

		if (isFreelook) OffsetYawPitch_Freelook();
		else OffsetYawPitch_Orbit();
	}

	void PerspectiveCameraController::OffsetYawPitch_Freelook() {
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_Camera->UpdateViewMatrix(m_Position, front);
	}

	void PerspectiveCameraController::OffsetYawPitch_Orbit() {
		float yawRad = glm::radians(m_Yaw);
		float pitchRad = glm::radians(m_Pitch);

		// Spherical to Cartesian coordinates conversion
		glm::vec3 position;
		position.x = m_Target.x + m_Radius * cos(pitchRad) * cos(yawRad);
		position.y = m_Target.y + m_Radius * sin(pitchRad);
		position.z = m_Target.z + m_Radius * cos(pitchRad) * sin(yawRad);

		m_Position = position;
		m_Camera->UpdateViewMatrix_T(m_Position, m_Target);
	}
}