#include "Texture2DTest.h"

using namespace GLCore;
using namespace GLCore::Utils;

Texture2DTest::Texture2DTest()
	: m_CameraController(16.0f / 9.0f)
{
	m_DebugName = "Example Layer";
}

Texture2DTest::~Texture2DTest() {
	delete m_va;
	delete m_vb;
	delete m_ib;
	delete m_Shader;
	delete m_Texture;
}

void Texture2DTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test_texture2D_vert.glsl",
		"assets/shaders/test_texture2D_frag.glsl"
	);
	m_Texture = new Texture("assets/textures/10.jpg");

	m_va = new VertexArray();

	m_vb = new VertexBuffer(vertices, sizeof(vertices));
	m_ib = new IndexBuffer(indices, sizeof(indices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_va->AddBuffer(*m_vb, layout);
}

void Texture2DTest::OnDetach()
{
	m_va->Unbind();
	m_vb->Unbind();
	m_ib->Unbind();
	m_Shader->Unbind();
	m_Texture->Unbind();
}

void Texture2DTest::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);
}

void Texture2DTest::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GLCore::Utils::Renderer::Clear();

	m_Shader->Bind();
	m_Texture->Bind(0);
	m_Shader->SetUniform1i("u_Texture", 0);
	m_Shader->SetUniformMat4f("u_MVP", m_CameraController.GetCamera().GetViewProjectionMatrix());

	m_va->Bind();
	GLCore::Utils::Renderer::Draw(*m_va, *m_ib, *m_Shader);
}

void Texture2DTest::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
