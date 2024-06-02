#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include <vector>

class PostProcessTest : public GLCore::Layer
{
public:
	PostProcessTest();
	virtual ~PostProcessTest();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::Shader* m_PostProcShader;
	GLCore::Utils::Model* m_Model;

	GLCore::Utils::VertexArray* m_VertexArray;
	GLCore::Utils::VertexBuffer* m_VertexBuffer;
	
	GLCore::Utils::PostProcess m_PostProcess;

	GLCore::Utils::PerspectiveCameraController m_CameraController;
	float m_time = 0.0f;

};