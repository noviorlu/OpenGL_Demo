#include "glpch.h"
#include "PerspectiveCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace GLCore::Utils {
	PerspectiveCameraController::PerspectiveCameraController()
		: CameraController()
	{
		m_Camera = new PerspectiveCamera();
	}

	PerspectiveCameraController::PerspectiveCameraController(float yaw, float pitch, glm::vec3 position)
		: CameraController(yaw, pitch, CameraController::CC_SPEED, CameraController::CC_SENSITIVITY)
	{
		m_Camera = new PerspectiveCamera(position);
		m_Position = position;
		OffsetYawPitch_Freelook();
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
		ImGui::Begin("Perspective Camera Controller");
		CameraController::OnImGuiRender();
		if(ImGui::DragFloat2("Yaw/Pitch", &m_Yaw, 0.0f)){
			OffsetYawPitch(0.0f, 0.0f);
		}

		ImGui::Text("Camera Control");
		ImGui::SliderFloat("Translation Speed", &m_CameraTranslationSpeed, 0.1f, 10.0f);
		ImGui::SliderFloat("Rotation Speed", &m_CameraRotationSpeed, 0.1f, 10.0f);

		if(ImGui::Checkbox("isFreelook? Or Orbit", &isFreelook)) {
			OffsetYawPitch(0.0f, 0.0f);
		}

		ImGui::Text("Camera Orbit");
		if (ImGui::InputFloat3("Camera Target", glm::value_ptr(m_Target))) {
			OffsetYawPitch(0.0f, 0.0f);
		}
		if (ImGui::SliderFloat("Orbit Radius", &m_Radius, 5.0f, 100.0f)) {
			OffsetYawPitch(0.0f, 0.0f);
		}
		ImGui::End();
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
		if(isFreelook) m_Pitch += offsetP;
		else m_Pitch -= offsetP;

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
		float yawRad = glm::radians(m_Yaw + 180);
		float pitchRad = glm::radians(m_Pitch);

		glm::vec3 front;
		front.x = cos(yawRad) * cos(pitchRad);
		front.y = sin(pitchRad);
		front.z = sin(yawRad) * cos(pitchRad);

		m_Camera->UpdateViewMatrix(m_Position, front);
	}

	void PerspectiveCameraController::OffsetYawPitch_Orbit() {
		float yawRad = glm::radians(m_Yaw);
		float pitchRad = glm::radians(m_Pitch);

		// Spherical to Cartesian coordinates conversion
		//glm::vec3 position;
		m_Position.x = m_Target.x + m_Radius * cos(pitchRad) * cos(yawRad);
		m_Position.y = m_Target.y + m_Radius * sin(pitchRad);
		m_Position.z = m_Target.z + m_Radius * cos(pitchRad) * sin(yawRad);

		//m_Position = position;
		m_Camera->UpdateViewMatrix_T(m_Position, m_Target);
	}
}