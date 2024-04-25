#include "ClearColorTest.h"

using namespace GLCore;
using namespace GLCore::Utils;

ClearColorTest::ClearColorTest()
	: m_CameraController(16.0f / 9.0f)
{
	m_DebugName = "Example Layer";
}

ClearColorTest::~ClearColorTest() {
	delete m_va;
	delete m_vb;
	delete m_ib;
	delete m_Shader;
}

void ClearColorTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	);


	m_va = new VertexArray();

	m_vb = new VertexBuffer(vertices, sizeof(vertices));
	m_ib = new IndexBuffer(indices, sizeof(indices));

	VertexBufferLayout layout;
	layout.Push<float>(3);

	m_va->AddBuffer(*m_vb, layout);
}

void ClearColorTest::OnDetach()
{
	m_va->Unbind();
	m_vb->Unbind();
	m_ib->Unbind();
	m_Shader->Unbind();
}

void ClearColorTest::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{
			m_SquareColor = m_SquareAlternateColor;
			return false;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
		{
			m_SquareColor = m_SquareBaseColor;
			return false;
		});
}

void ClearColorTest::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	GLCore::Utils::Renderer::Clear();

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_ViewProjection", m_CameraController.GetCamera()->GetViewProjectionMatrix());
	m_Shader->SetUniform4fv("u_Color", m_SquareColor);

	m_va->Bind();
	GLCore::Utils::Renderer::Draw(*m_va, *m_ib, *m_Shader);
}

void ClearColorTest::OnImGuiRender()
{
	ImGui::Begin("Controls");
	if (ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareBaseColor)))
		m_SquareColor = m_SquareBaseColor;
	ImGui::ColorEdit4("Square Alternate Color", glm::value_ptr(m_SquareAlternateColor));
	ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
	ImGui::End();
}
