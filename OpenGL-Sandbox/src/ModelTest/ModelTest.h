#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class ModelTest : public GLCore::Layer
{
public:
	ModelTest();
	virtual ~ModelTest();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::Model* m_Model;

	GLCore::Utils::PerspectiveCameraController m_CameraController;
	float m_time = 0.0f;

};