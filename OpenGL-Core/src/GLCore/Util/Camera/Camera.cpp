#include "glpch.h"
#include "Camera.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace GLCore::Utils {
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
            m_fullScreenWidth * m_viewPortX,
            m_fullScreenHeight * m_viewPortY, 
            m_fullScreenWidth * m_viewPortWidth,
            m_fullScreenHeight * m_viewPortHeight
        );
	}
}