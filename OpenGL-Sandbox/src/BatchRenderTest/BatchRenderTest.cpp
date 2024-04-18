#include "BatchRenderTest.h"

using namespace GLCore;
using namespace GLCore::Utils;

BatchRenderTest::BatchRenderTest()
	: m_CameraController(16.0f / 9.0f)
{
	m_DebugName = "BatchRender Layer";
}

BatchRenderTest::~BatchRenderTest() {
	delete m_va;
	delete m_vb;
	delete m_ib;
	delete m_Shader;
	delete m_Texture1;
	delete m_Texture2;
}

void BatchRenderTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float vertices[] = {
	//  | positions         | colors                   |uv         |uvIdx
		-0.55f, 0.05f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.05f, 0.05f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.05f, 0.55f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.55f, 0.55f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, -1.0f,
																   
		 0.05f, 0.05f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.55f, 0.05f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.55f, 0.55f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.05f, 0.55f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, -1.0f,

		-0.55f, -0.55f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.05f, -0.55f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.05f, -0.05f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.55f, -0.05f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 0.0f,

		 0.05f, -0.55f, 0.0f, 0.11f, 0.6f, 0.76f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.55f, -0.55f, 0.0f, 0.11f, 0.6f, 0.76f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.55f, -0.05f, 0.0f, 0.11f, 0.6f, 0.76f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.05f, -0.05f, 0.0f, 0.11f, 0.6f, 0.76f, 1.0f, 0.0f, 1.0f, 1.0f

	};

	uint32_t indices[] = { 
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12
	};

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test_Batch_vert.glsl",
		"assets/shaders/test_Batch_frag.glsl"
	);

	m_Texture1 = new Texture("assets/textures/10.jpg");
	m_Texture2 = new Texture("assets/textures/47.jpg");

	m_va = new VertexArray();

	m_vb = new VertexBuffer(vertices, sizeof(vertices));
	m_ib = new IndexBuffer(indices, sizeof(indices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);
	layout.Push<float>(2);
	layout.Push<float>(1);

	m_va->AddBuffer(*m_vb, layout);

	m_Shader->Bind();
	m_Texture1->Bind(0);
	m_Texture2->Bind(1);
	int samplers[2] = { 0, 1 };
	m_Shader->SetUniform1iv("u_Textures", 2, samplers);
}

void BatchRenderTest::OnDetach()
{
	m_va->Unbind();
	m_vb->Unbind();
	m_ib->Unbind();
	m_Shader->Unbind();
	m_Texture1->Unbind();
	m_Texture2->Unbind();
}

void BatchRenderTest::OnEvent(Event& event)
{	
	m_CameraController.OnEvent(event);
}

void BatchRenderTest::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	GLCore::Utils::Renderer::Clear();

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());

	m_va->Bind();
	m_Shader->SetUniformMat4f("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f) ));
	GLCore::Utils::Renderer::Draw(*m_va, *m_ib, *m_Shader);
}

void BatchRenderTest::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	ImGui::End();
}
