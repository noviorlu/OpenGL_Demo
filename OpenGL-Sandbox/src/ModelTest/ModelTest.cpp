#include "ModelTest.h"


using namespace GLCore;
using namespace GLCore::Utils;

ModelTest::ModelTest()
	: m_CameraController(270,0,glm::vec3(0,0,20))
{
	m_DebugName = "Model Test";
}

ModelTest::~ModelTest() {
	delete m_Shader;
	delete m_Model;
}

void ModelTest::OnAttach()
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
		"assets/models/mary/Mary.obj",
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.015, 0.015, 0.015)
	);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

	m_va = new VertexArray();

	m_vb = new VertexBuffer(vertices, sizeof(vertices));
	m_ib = new IndexBuffer(indices, sizeof(indices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_va->AddBuffer(*m_vb, layout);
}

void ModelTest::OnDetach()
{
	delete m_Shader;
	delete m_Model;
}

void ModelTest::OnEvent(Event& event){
	m_CameraController.OnEvent(event);
}

void ModelTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController.OnUpdate(ts);
	
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GLCore::Utils::Renderer::Clear();

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("ViewProjection", m_CameraController.GetCamera()->GetViewProjectionMatrix());

	
	m_Shader->SetUniformMat4f("model", m_Model->GetModelMatrix());
	m_Model->Draw(*m_Shader);
	GLCore::Utils::Renderer::Draw(*m_va, *m_ib, *m_Shader);
}

void ModelTest::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	m_CameraController.OnImGuiRender();
	ImGui::End();
}
