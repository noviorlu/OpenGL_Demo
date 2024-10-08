#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include <vector>

class BlinnPhongTest : public GLCore::Layer
{
public:
	BlinnPhongTest();
	virtual ~BlinnPhongTest();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::Shader* m_IconShader;
	GLCore::Utils::Model* m_Model;
	GLCore::Utils::LightGroup* m_LightGroup;

	GLCore::Utils::PerspectiveCameraController* m_CameraController;
	float m_time = 0.0f;

};