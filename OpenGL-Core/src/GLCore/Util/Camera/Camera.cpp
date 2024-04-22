#include "glpch.h"
#include "Camera.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace GLCore::Utils {
    void Camera::OffsetYawPitch(const float& offsetY, const float& offsetP) { 
        m_Yaw += offsetY; 
        m_Pitch += offsetP; 

        if (m_Yaw > 360.0f) m_Yaw = 0.0f;
        else if (m_Yaw < 0.0f) m_Yaw = 360.0f;

        if (m_Pitch < -180.0f) m_Pitch = 180.0f;
        else if (m_Pitch > 180.0f) m_Pitch = -180.0f;

        if (m_Pitch > 90.0f || m_Pitch < -90.0f)
            m_WorldUp = WORLD_DOWN;
        else
            m_WorldUp = WORLD_UP;

        RecalculateViewMatrix(); 
    }

    
    void Camera::OnWindowResized(uint32_t width, uint32_t height) {
        m_fullScreenWidth = width;
        m_fullScreenHeight = height;
        SetViewPort();
    }
    

    void Camera::RecalculateViewMatrix()
	{
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

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
        if(ImGui::DragFloat2("Yaw and Pitch", &m_Yaw))
            RecalculateViewMatrix();
        
        if(ImGui::DragFloat2("ViewPort XY Perc", &m_viewPortX, 0, 1))
			SetViewPort();
		if(ImGui::DragFloat2("ViewPort WH Perc", &m_viewPortWidth, 0, 1))
            SetViewPort();
    }
}