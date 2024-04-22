#include "glpch.h"
#include "CameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include <imgui.h>

namespace GLCore::Utils {

	void CameraController::OnUpdate(Timestep ts)
	{
		bool recalcView = false;
		m_tempPosition = m_Camera->GetPosition();

		if (Input::IsKeyPressed(HZ_KEY_A))
		{
			m_tempPosition -= (m_CameraTranslationSpeed * ts) * m_Camera->GetRight();
			recalcView = true;
		}
		else if (Input::IsKeyPressed(HZ_KEY_D))
		{
			m_tempPosition += (m_CameraTranslationSpeed * ts) * m_Camera->GetRight();
			recalcView = true;
		}

		if (Input::IsKeyPressed(HZ_KEY_W))
		{
			m_tempPosition += (m_CameraTranslationSpeed * ts) * m_Camera->GetFront();
			recalcView = true;
		}
		else if (Input::IsKeyPressed(HZ_KEY_S))
		{
			m_tempPosition -= (m_CameraTranslationSpeed * ts) * m_Camera->GetFront();
			recalcView = true;
		}

		if (Input::IsKeyPressed(HZ_KEY_Q))
		{
			m_tempPosition += (m_CameraTranslationSpeed * ts) * m_Camera->GetUp();
			recalcView = true;
		}
		else if (Input::IsKeyPressed(HZ_KEY_E))
		{
			m_tempPosition -= (m_CameraTranslationSpeed * ts) * m_Camera->GetUp();
			recalcView = true;
		}
		if(recalcView) m_Camera->SetPosition(m_tempPosition);
	}

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

		ImGui::Text("Camera Controller");
		ImGui::SliderFloat("Translation Speed", &m_CameraTranslationSpeed, 0.1f, 10.0f);
		ImGui::SliderFloat("Rotation Speed", &m_CameraRotationSpeed, 0.1f, 10.0f);
	}
}