#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class BatchRenderTest : public GLCore::Layer
{
public:
	BatchRenderTest();
	virtual ~BatchRenderTest();

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
	GLCore::Utils::Texture* m_Texture1;
	GLCore::Utils::Texture* m_Texture2;

	glm::vec4 m_ClearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
};