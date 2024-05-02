#include "glpch.h"
#include "CameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace GLCore::Utils {
	void CameraController::OnUpdate(Timestep ts){ }

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(CameraController::OnWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(GLCORE_BIND_EVENT_FN(CameraController::OnMousePressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(GLCORE_BIND_EVENT_FN(CameraController::OnMouseReleased));
	}

	void CameraController::OnImGuiRender()
	{
		m_Camera->OnImGuiRender();

		ImGui::Text("Camera Transform");
		if (ImGui::InputFloat3("Camera Position", glm::value_ptr(m_Position)))
			m_Camera->SetPosition(m_Position);
	}

	void CameraController::KeyboardMovement(Timestep ts) {
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_Position -= (m_CameraTranslationSpeed * ts) * m_Camera->GetRight();
		else if (Input::IsKeyPressed(HZ_KEY_D))
			m_Position += (m_CameraTranslationSpeed * ts) * m_Camera->GetRight();

		if (Input::IsKeyPressed(HZ_KEY_W))
			m_Position += (m_CameraTranslationSpeed * ts) * m_Camera->GetFront();
		else if (Input::IsKeyPressed(HZ_KEY_S))
			m_Position -= (m_CameraTranslationSpeed * ts) * m_Camera->GetFront();

		if (Input::IsKeyPressed(HZ_KEY_Q))
			m_Position += (m_CameraTranslationSpeed * ts) * m_Camera->GetUp();
		else if (Input::IsKeyPressed(HZ_KEY_E))
			m_Position -= (m_CameraTranslationSpeed * ts) * m_Camera->GetUp();
		m_Camera->SetPosition(m_Position);
	}
}