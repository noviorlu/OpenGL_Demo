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
		CameraController::OnUpdate(ts);

		if (m_LeftMousePressed)
		{
			auto [x, y] = Input::GetMousePosition();
			float xoffset = x - m_LastX;
			float yoffset = m_LastY - y;
			m_LastX = x;
			m_LastY = y;

			xoffset *= m_MouseSensitivity;
			yoffset *= m_MouseSensitivity;

			m_Camera->OffsetYawPitch(xoffset, yoffset);
		}
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

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_Camera->OnWindowResized(e.GetWidth(), e.GetHeight());
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
}