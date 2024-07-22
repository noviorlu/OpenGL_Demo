#include "PPMTest.h"


using namespace GLCore;
using namespace GLCore::Utils;

PPMTest::PPMTest()
{
	m_DebugName = "Model Test";
}

PPMTest::~PPMTest() {
	delete m_Model;
	delete m_Shader;
	
}

void PPMTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/RTX/RTX_vert.glsl",
		"assets/shaders/RTX/RTX_frag.glsl"
	);

	m_CameraController = new PerspectiveCameraController(0, 0, glm::vec3(20, 0, 0));

	m_Model = new Model(
		glm::vec3(0.0, -2.0, 0.0),
		glm::vec3(180.0, 0.0, 0.0),
		glm::vec3(2, 2, -2)
	);
	{ glTFLoader ldr = glTFLoader("assets/models/cornellBox/CornellBoxOriginal.gltf", m_Model); }
	m_Model->ConvertToBlinnPhongMaterial();

	std::vector<float> SceneData;
	std::vector<float> MatData;

	m_Model->SeralizeMaterial(MatData, 0);
	m_Model->Seralize(SceneData, 0);

	int SceneTexSize = ceil(sqrt(SceneData.size()));
	int MatTexSize = ceil(sqrt(MatData.size()));

	m_SceneTexture = new Texture(SceneTexSize, SceneTexSize, GL_RGBA, GL_FLOAT, SceneData.data());
	m_MaterialTexture = new Texture(MatTexSize, MatTexSize, GL_RGBA, GL_FLOAT, MatData.data());

	m_Quad = new Quad();
}

void PPMTest::OnDetach()
{
}

void PPMTest::OnEvent(Event& event) {
	m_CameraController->OnEvent(event);
}

void PPMTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController->OnUpdate(ts);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_Shader->Bind();
	m_Quad->Draw(*m_Shader);

}

void PPMTest::OnImGuiRender()
{
	ImGui::Text("Application average\n %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//m_CameraController->OnImGuiRender();
	//m_Model->OnImGuiRender();
}
