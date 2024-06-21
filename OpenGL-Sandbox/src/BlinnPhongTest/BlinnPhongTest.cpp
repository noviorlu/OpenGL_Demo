#include "BlinnPhongTest.h"


using namespace GLCore;
using namespace GLCore::Utils;

BlinnPhongTest::BlinnPhongTest()
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

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/blinnphong_vert.glsl",
		"assets/shaders/blinnphong_frag.glsl"
	);

	m_IconShader = Shader::FromGLSLTextFiles(
		"assets/shaders/cube_vert.glsl",
		"assets/shaders/cube_frag.glsl"
	);

	m_CameraController = new PerspectiveCameraController(0, 0, glm::vec3(20, 0, 0));

	m_Model = new Model(
		glm::vec3(0.0, -2.0, 0.0),
		glm::vec3(180.0, 0.0, 0.0),
		glm::vec3(2, 2, -2)
	);

	{ glTFLoader ldr = glTFLoader("assets/models/mary/mary.gltf", m_Model); }
	m_Model->ConvertToBlinnPhongMaterial();
	auto mat = std::dynamic_pointer_cast<BlinnPhongMaterial>(m_Model->m_MaterialPool["MC003_Kozakura_Mari_BlinnPhong"]);
	mat->m_Diffuse = glm::vec3(141.0f/255.0f, 70.0f / 255.0f, 70.0f / 255.0f);
	mat->m_Shininess = 0.0f;

	m_LightGroup = new BlinnPhongLightGroup(
		glm::vec3(78.0f/255.0f, 86.0f/255.0f, 120.0f/255.0f),
		0.1f
	);
	m_LightGroup->AddLight(
		std::make_shared<BlinnPhongLight>(
			LightType::POINT,
			glm::vec3(10.0f, 10.0f, 10.0f),
			glm::vec3(1.0f, 224.0f / 255.0f, 190.0f / 255.0f),
			160.0f
		)
	);
}

void BlinnPhongTest::OnDetach()
{
}

void BlinnPhongTest::OnEvent(Event& event) {
	m_CameraController->OnEvent(event);
}

void BlinnPhongTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController->OnUpdate(ts);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_Shader->Bind();
	m_CameraController->GetCamera()->Draw(*m_Shader);
	m_LightGroup->Draw(*m_Shader);
	m_Model->Draw(*m_Shader);

	m_IconShader->Bind();
	m_CameraController->GetCamera()->Draw(*m_IconShader);
	m_LightGroup->DrawIcon(*m_IconShader);
}

void BlinnPhongTest::OnImGuiRender()
{
	ImGui::Text("Application average\n %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	m_CameraController->OnImGuiRender();
	m_Model->OnImGuiRender();
	m_LightGroup->OnImGuiRender();
}
