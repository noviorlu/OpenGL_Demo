#pragma once

#include <glm/glm.hpp>

#include "GLCore/Core/Application.h"

namespace GLCore::Utils {

	class Camera
	{
	public:
		static constexpr glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
		static constexpr glm::vec3 DEFAULT_LOOKAT = glm::vec3(0.0f, 0.0f, 0.0f);
		static constexpr glm::vec3 DEFAULT_POS = glm::vec3(0.0f, 0.0f, 10.0f);
		static constexpr glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
		static constexpr glm::vec3 DEFAULT_DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
	
		Camera(glm::vec3 position = DEFAULT_POS, glm::vec3 lookAt = DEFAULT_LOOKAT);

		/* Camera Getters */
		const glm::vec3& GetPosition() const { return m_Position; }

		const glm::vec3& GetFront() const { return m_Front; }
		const glm::vec3& GetUp() const { return m_Up; }
		const glm::vec3& GetRight() const { return m_Right; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		const float GetViewPortX() const { return m_viewPortX; }
		const float GetViewPortY() const { return m_viewPortY; }
		const float GetViewPortWidth() const { return m_viewPortWidth; }
		const float GetViewPortHeight() const { return m_viewPortHeight; }
		
		void SetWorldUp(bool isUp) { m_WorldUp = isUp ? DEFAULT_UP : DEFAULT_DOWN; }
		void SetViewPortX(float x) { m_viewPortX = x; SetViewPort(); }
		void SetViewPortY(float y) { m_viewPortY = y; SetViewPort(); }
		void SetViewPortWidth(float width) { m_viewPortWidth = width; SetViewPort(); }
		void SetViewPortHeight(float height) { m_viewPortHeight = height; SetViewPort(); }

		/* Camera Public Functions */
		virtual void OnWindowResized(uint32_t width, uint32_t height);
		virtual void OnImGuiRender();
		
		void UpdateViewMatrix(const glm::vec3& position, const glm::vec3& Front);
		void UpdateViewMatrix_T(const glm::vec3& position, const glm::vec3& lookAt);
		
	protected:
		void RecalculateViewMatrix();
		virtual void RecalculateProjectionMatrix() = 0;
		void RecalculateVPMatrix();
		void SetViewPort();
		
	protected:
		/* Camera View Info */
		glm::vec3 m_Position = { 0.0f,  0.0f,  0.0f };
		glm::vec3 m_Front = { 0.0f,  0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f,  1.0f,  0.0f };
		glm::vec3 m_Right = { 1.0f,  0.0f,  0.0f };
		glm::vec3 m_WorldUp = { 0.0f,  1.0f,  0.0f };


		/* Camera Matrices */
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		

		/* Camera glViewport Info [in percentage] */
		float m_viewPortX = 0.0f;
		float m_viewPortY = 0.0f;
		float m_viewPortWidth = 1.0f;
		float m_viewPortHeight = 1.0f;

		uint32_t m_fullScreenWidth = 1600;
		uint32_t m_fullScreenHeight = 900;
	};
}
