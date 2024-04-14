#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class ClearColorTest : public GLCore::Layer
{
public:
	ClearColorTest();
	virtual ~ClearColorTest();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::VertexArray* m_va;
	GLCore::Utils::VertexBuffer* m_vb;
	GLCore::Utils::IndexBuffer* m_ib;
	GLCore::Utils::OrthographicCameraController m_CameraController;
	
	GLuint m_QuadVA, m_QuadVB, m_QuadIB;

	glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareAlternateColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor = m_SquareBaseColor;
	glm::vec4 m_ClearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
};