#include "glpch.h"
#include "PerspectiveCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include <imgui.h>
namespace GLCore::Utils {

	PerspectiveCameraController::PerspectiveCameraController() 
		: PerspectiveCamera(){}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		bool recalcView = false;
		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_Position -= (m_CameraTranslationSpeed * ts) * m_Right;
			recalcView = true;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_Position += (m_CameraTranslationSpeed * ts) * m_Right;
			recalcView = true;
		}

		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_Position += (m_CameraTranslationSpeed * ts) * m_Front;
			recalcView = true;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_Position -= (m_CameraTranslationSpeed * ts) * m_Front;
			recalcView = true;
		}

		if (Input::IsKeyPressed(HZ_KEY_Q))
		{
			m_Position -= (m_CameraTranslationSpeed * ts) * m_Up;
			recalcView = true;
		}
		else if (Input::IsKeyPressed(HZ_KEY_E))
		{
			m_Position += (m_CameraTranslationSpeed * ts) * m_Up;
			recalcView = true;
		}
		if(recalcView) RecalculateViewMatrix();

		if (m_LeftMousePressed)
		{
			auto [x, y] = Input::GetMousePosition();
			float xoffset = x - m_LastX;
			float yoffset = m_LastY - y;
			m_LastX = x;
			m_LastY = y;

			xoffset *= m_MouseSensitivity;
			yoffset *= m_MouseSensitivity;

			m_Yaw += xoffset;
			m_Pitch += yoffset;

			if(m_Yaw > 360.0f) m_Yaw = 0.0f;
			else if(m_Yaw < 0.0f) m_Yaw = 360.0f;
			
			if(m_Pitch < -180.0f) m_Pitch = 180.0f;
			else if(m_Pitch > 180.0f) m_Pitch = -180.0f;

			if (m_Pitch > 90.0f || m_Pitch < -90.0f)
				m_WorldUp = WORLD_DOWN;
			else
				m_WorldUp = WORLD_UP;

			RecalculateViewMatrix();
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(GLCORE_BIND_EVENT_FN(PerspectiveCameraController::OnMouseReleased));
	}

	void PerspectiveCameraController::OnImGuiRender()
	{
		PerspectiveCamera::OnImGuiRender();
		ImGui::Text("Perspective Camera Controller");
		ImGui::SliderFloat("Camera Translation Speed", &m_CameraTranslationSpeed, 0.1f, 10.0f);
		ImGui::SliderFloat("Mouse Sensitivity", &m_MouseSensitivity, 0.1f, 10.0f);
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (m_Fov >= 1.0f && m_Fov <= 160.0f)
			m_Fov -= e.GetYOffset();
		RecalculateProjectionMatrix();
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		PerspectiveCamera::OnWindowResized(e.GetWidth(), e.GetHeight());
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