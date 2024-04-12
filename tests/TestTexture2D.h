#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include <memory>

namespace test {
	class TestTexture2D : public Test {
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr <VertexBuffer> m_vb;
		std::unique_ptr <IndexBuffer> m_ib;
		std::unique_ptr <Shader> m_shader;
		std::unique_ptr <Texture> m_texture;
		glm::mat4 m_projMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_modelMatrix;
		glm::vec3 m_translationA;
		glm::vec3 m_translationB;
	};
	
}