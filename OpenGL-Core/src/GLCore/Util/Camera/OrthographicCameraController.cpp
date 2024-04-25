#include "glpch.h"
#include "OrthographicCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"

namespace GLCore::Utils {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio)
	{
		m_Camera = new OrthographicCamera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		KeyboardMovement(ts);
		m_Camera->UpdateViewMatrix(m_Position, Camera::DEFAULT_FRONT);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		((OrthographicCamera*)m_Camera)->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		((OrthographicCamera*)m_Camera)->SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		m_Camera->OnWindowResized(e.GetWidth(), e.GetHeight());
		return false;
	}
}