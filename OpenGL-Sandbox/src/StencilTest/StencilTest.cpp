#include "StencilTest.h"


using namespace GLCore;
using namespace GLCore::Utils;

StencilTest::StencilTest()
	: m_CameraController(0, 0, glm::vec3(20, 0, 0))
{
	m_DebugName = "Stencil Test";
}

StencilTest::~StencilTest() {
	delete m_Model;
	delete m_Shader;
	delete m_Outline;
}

void StencilTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/model_loading_vert.glsl",
		"assets/shaders/pbr_frag.glsl"
	);

	m_Outline = Shader::FromGLSLTextFiles(
		"assets/shaders/model_loading_vert.glsl",
		"assets/shaders/outline_frag.glsl"
	);

	m_Model = new Model(
		glm::vec3(0.0, -3.0, 0.0),
		glm::vec3(180.0, 0.0, 0.0),
		glm::vec3(2, 2, -2)
	);
	{ glTFLoader ldr = glTFLoader("assets/models/mary/mary.gltf", m_Model); }
}

void StencilTest::OnDetach()
{
	glStencilMask(0xFF);
	Renderer::Clear();
	glStencilMask(0x00); // disable writing to the stencil buffer
}

void StencilTest::OnEvent(Event& event){
	m_CameraController.OnEvent(event);
}

void StencilTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController.OnUpdate(ts);
	
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	Renderer::Clear();

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("ViewProjection", m_CameraController.GetCamera()->GetViewProjectionMatrix());
	m_Outline->Bind();
	m_Outline->SetUniformMat4f("ViewProjection", m_CameraController.GetCamera()->GetViewProjectionMatrix());

	// Setup Stencil Buffer Write rule
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
	
	m_Model->Draw(*m_Shader);
	
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00); // disable writing to the stencil buffer
	glDisable(GL_DEPTH_TEST);

	m_Model->m_Transform.SetScale(glm::vec3(2.1, 2.1, -2.1));
	m_Model->m_Transform.SetTranslation(glm::vec3(0.0, -3.1, 0.0));
	m_Model->Draw(*m_Outline);
	m_Model->m_Transform.SetScale(glm::vec3(2, 2, -2));
	m_Model->m_Transform.SetTranslation(glm::vec3(0.0, -3.0, 0.0));
}

void StencilTest::OnImGuiRender()
{
	ImGui::Text("Application average\n %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	m_CameraController.OnImGuiRender();
	m_Model->OnImGuiRender();
}
