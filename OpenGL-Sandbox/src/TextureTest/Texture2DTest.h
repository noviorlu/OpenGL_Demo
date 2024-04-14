#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class Texture2DTest : public GLCore::Layer
{
public:
	Texture2DTest();
	virtual ~Texture2DTest();

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
	GLCore::Utils::Texture* m_Texture;
	GLCore::Utils::OrthographicCameraController m_CameraController;
};