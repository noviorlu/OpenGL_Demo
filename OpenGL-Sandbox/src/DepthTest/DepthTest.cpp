#include "DepthTest.h"


using namespace GLCore;
using namespace GLCore::Utils;

DepthTest::DepthTest()
	: m_CameraController(0,0,glm::vec3(0,0,20.0))
{
	m_DebugName = "Model Test";
}

DepthTest::~DepthTest() {
	delete m_Model;
	delete m_Shader;
}

void DepthTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/model_loading_vert.glsl",
		"assets/shaders/depth_frag.glsl"
	);

	m_Model = new Model(
		glm::vec3(0.0, -3.0, 0.0),
		glm::vec3(180.0, 0.0, 0.0),
		glm::vec3(0.8, 0.8, 0.8)
	);
	{ glTFLoader ldr = glTFLoader("assets/models/sponza/sponza.gltf", m_Model); }
}

void DepthTest::OnDetach()
{
}

void DepthTest::OnEvent(Event& event){
	m_CameraController.OnEvent(event);
}

void DepthTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController.OnUpdate(ts);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GLCore::Utils::Renderer::Clear();

	m_Shader->Bind();
	m_CameraController.GetCamera()->Draw(*m_Shader);
	//m_Shader->SetUniformMat4f("u_ViewProjection", m_CameraController.GetCamera()->GetViewProjectionMatrix());
	m_Model->Draw(*m_Shader);
}

void DepthTest::OnImGuiRender()
{
	ImGui::Text("Application average\n %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	m_CameraController.OnImGuiRender();
	m_Model->OnImGuiRender();
}
