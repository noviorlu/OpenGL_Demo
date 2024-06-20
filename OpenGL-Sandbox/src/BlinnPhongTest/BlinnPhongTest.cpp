#include "BlinnPhongTest.h"


using namespace GLCore;
using namespace GLCore::Utils;

BlinnPhongTest::BlinnPhongTest()
	: m_CameraController(0, 0, glm::vec3(20, 0, 0))
{
	m_DebugName = "Model Test";
}

BlinnPhongTest::~BlinnPhongTest() {
	delete m_Model;
	delete m_Shader;
	
}

void BlinnPhongTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// configure global opengl state, depth comparisons and update the depth buffer
	// https://docs.gl/gl4/glEnable
	glEnable(GL_DEPTH_TEST);


	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/model_loading_vert.glsl",
		"assets/shaders/model_loading_frag.glsl"
	);

	m_Model = new Model(
		glm::vec3(0.0, -2.0, 0.0),
		glm::vec3(180.0, 0.0, 0.0),
		glm::vec3(2, 2, 2)
	);
	{ glTFLoader ldr = glTFLoader("assets/models/mary/mary.gltf", m_Model); }
	//m_Model = new Model(
	//	glm::vec3(0.0, 0.0, 0.0),
	//	glm::vec3(180.0, 0.0, 0.0),
	//	glm::vec3(0.03, 0.03, 0.03)
	//);
	//{ glTFLoader ldr = glTFLoader("assets/models/scroll/scene.gltf", m_Model); }
	//m_Model = new Model(
	//	glm::vec3(0.0, 0.0, 0.0),
	//	glm::vec3(180.0, 0.0, 0.0),
	//	glm::vec3(0.03, 0.03, 0.03)
	//);
	//{ glTFLoader ldr = glTFLoader("assets/models/sword/scene.gltf", m_Model); }
}

void BlinnPhongTest::OnDetach()
{
}

void BlinnPhongTest::OnEvent(Event& event) {
	m_CameraController.OnEvent(event);
}

void BlinnPhongTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController.OnUpdate(ts);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GLCore::Utils::Renderer::Clear();

	m_Shader->Bind();
	m_CameraController.GetCamera()->Draw(*m_Shader);
	m_Model->Draw(*m_Shader);
}

void BlinnPhongTest::OnImGuiRender()
{
	ImGui::Text("Application average\n %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	m_CameraController.OnImGuiRender();
	m_Model->OnImGuiRender();
}
