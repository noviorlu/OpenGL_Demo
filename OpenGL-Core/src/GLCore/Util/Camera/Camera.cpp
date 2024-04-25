#include "glpch.h"
#include "Camera.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace GLCore::Utils {
    Camera::Camera(glm::vec3 position, glm::vec3 lookAt)
    {
        UpdateViewMatrix(position, lookAt);
        const auto& win = Application::Get().GetWindow();
        m_fullScreenWidth = win.GetWidth();
        m_fullScreenHeight = win.GetHeight();
    }

    void Camera::OnWindowResized(uint32_t width, uint32_t height) {
        m_fullScreenWidth = width;
        m_fullScreenHeight = height;
        SetViewPort();
    }
    
    void Camera::UpdateViewMatrix(const glm::vec3& position, const glm::vec3& front)
	{
        m_Position = position;
        m_Front = glm::normalize(front);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        RecalculateViewMatrix();
    }

    void Camera::UpdateViewMatrix_T(const glm::vec3& position, const glm::vec3& lookAt)
    {
        m_Position = position;
        m_Front = glm::normalize(lookAt - position);
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        RecalculateViewMatrix();
    }

    void Camera::RecalculateViewMatrix()
    {
        m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        RecalculateVPMatrix();
    }

    void Camera::RecalculateVPMatrix()
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

    void Camera::SetViewPort()
	{
        glViewport(
            (int)(m_fullScreenWidth * m_viewPortX),
            (int)(m_fullScreenHeight * m_viewPortY),
            (int)(m_fullScreenWidth * m_viewPortWidth),
            (int)(m_fullScreenHeight * m_viewPortHeight)
        );
	}

    void Camera::OnImGuiRender() {
        ImGui::Text("Camera");
        if(ImGui::InputFloat3("Camera Position", glm::value_ptr(m_Position)))
			RecalculateViewMatrix();
        //if(ImGui::DragFloat2("Yaw and Pitch", &m_Yaw))
        //    RecalculateViewMatrix();
        
        if(ImGui::DragFloat2("ViewPort XY Perc", &m_viewPortX, 0, 1))
			SetViewPort();
		if(ImGui::DragFloat2("ViewPort WH Perc", &m_viewPortWidth, 0, 1))
            SetViewPort();
    }
}