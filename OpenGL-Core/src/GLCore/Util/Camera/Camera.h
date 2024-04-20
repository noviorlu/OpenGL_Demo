#pragma once

#include <glm/glm.hpp>

#include "GLCore/Core/Application.h"

namespace GLCore::Utils {
	const glm::vec3 WORLD_POS = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 WORLD_DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
	const float YAW = 270.0f;
	const float PITCH = 0.0f;

	class Camera
	{
	public:
		Camera(glm::vec3 position = WORLD_POS, glm::vec3 up = WORLD_UP, float yaw = YAW, float pitch = PITCH) 
			: m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch)
		{
			RecalculateViewMatrix();
			const auto& win = Application::Get().GetWindow();
			m_fullScreenWidth = win.GetWidth();
			m_fullScreenHeight = win.GetHeight();
		}

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		const float GetViewPortX() const { return m_viewPortX; }
		const float GetViewPortY() const { return m_viewPortY; }
		const float GetViewPortWidth() const { return m_viewPortWidth; }
		const float GetViewPortHeight() const { return m_viewPortHeight; }

		void SetViewPortX(float x) {
			m_viewPortX = x; 
			SetViewPort();
		}
		void SetViewPortY(float y) { 
			m_viewPortY = y; 
			SetViewPort();
		}
		void SetViewPortWidth(float width) { 
			m_viewPortWidth = width;
			SetViewPort();
		}
		void SetViewPortHeight(float height) { 
			m_viewPortHeight = height; 
			SetViewPort();
		}
		virtual void OnWindowResized(uint32_t width, uint32_t height) {
			m_fullScreenWidth = width;
			m_fullScreenHeight = height;
			SetViewPort();
		}
	protected:
		void RecalculateViewMatrix();
		virtual void RecalculateProjectionMatrix() = 0;
		void RecalculateVPMatrix();
		void SetViewPort();
		
		virtual void OnImGuiRender();

	protected:
		/* Camera Matrices */
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		/* Camera View Info */
		glm::vec3 m_Position	=	{ 0.0f,  0.0f,  0.0f };
		glm::vec3 m_Front		=	{ 0.0f,  0.0f, -1.0f };
		glm::vec3 m_Up			=	{ 0.0f,  1.0f,  0.0f };
		glm::vec3 m_Right		=	{ 1.0f,  0.0f,  0.0f };
		glm::vec3 m_WorldUp		=	{ 0.0f,  1.0f,  0.0f };
		
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;

		/* Camera glViewport Info [in percentage] */
		float m_viewPortX = 0.0f;
		float m_viewPortY = 0.0f;
		float m_viewPortWidth = 1.0f;
		float m_viewPortHeight = 1.0f;

		uint32_t m_fullScreenWidth = 1600;
		uint32_t m_fullScreenHeight = 900;
	};
}
