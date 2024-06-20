#include "CubeTest.h"

using namespace GLCore;
using namespace GLCore::Utils;

CubeTest::CubeTest()
	: m_CameraController(270,0,glm::vec3(0,0,3))
{
	m_DebugName = "Cube Test";
}

CubeTest::~CubeTest() {
	delete m_va;
	delete m_vb;
	delete m_Shader;
	delete m_Texture;
}

void CubeTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// configure global opengl state, depth comparisons and update the depth buffer
	// https://docs.gl/gl4/glEnable
	glEnable(GL_DEPTH_TEST);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	

	m_va = new VertexArray();
	m_vb = new VertexBuffer(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_va->AddBuffer(*m_vb, layout);


	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test_cube_vert.glsl",
		"assets/shaders/test_texture2D_frag.glsl"
	);
	m_Texture = new Texture("assets/textures/10.jpg");

	m_Shader->Bind();
	m_Texture->Bind(0);
	m_Shader->SetUniform1i("u_Texture", 0);
}

void CubeTest::OnDetach()
{
	m_va->Unbind();
	m_vb->Unbind();
	m_Shader->Unbind();
	m_Texture->Unbind();
}

void CubeTest::OnEvent(Event& event){
	m_CameraController.OnEvent(event);
}

void CubeTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController.OnUpdate(ts);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GLCore::Utils::Renderer::Clear();

	m_Texture->Bind(0);
	m_Shader->Bind();

	m_Shader->SetUniformMat4f("u_ViewProjection", m_CameraController.GetCamera()->GetViewProjectionMatrix());

	for (unsigned int i = 1; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i * m_time;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_Shader->SetUniformMat4f("model", model);

		// render the cube
		Renderer::Draw(*m_va, *m_vb, *m_Shader);
	}
}

void CubeTest::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	m_CameraController.OnImGuiRender();
	ImGui::End();
}
