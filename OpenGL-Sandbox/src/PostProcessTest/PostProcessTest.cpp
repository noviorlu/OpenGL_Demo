#include "PostProcessTest.h"


using namespace GLCore;
using namespace GLCore::Utils;

PostProcessTest::PostProcessTest()
	: m_CameraController(0,0,glm::vec3(20,0,0))
{
	m_DebugName = "PostProcess Test";
}

PostProcessTest::~PostProcessTest() {
		delete m_Model;
		delete m_Shader;
		delete m_PostProcShader;
		delete m_VertexArray;
		delete m_VertexBuffer;
}

void PostProcessTest::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/model_loading_vert.glsl",
		"assets/shaders/pbr_frag.glsl"
	);
	m_PostProcShader = Shader::FromGLSLTextFiles(
		"assets/shaders/postproc_vert.glsl",
		"assets/shaders/postproc_frag.glsl"
	);

	m_Model = new Model(
		glm::vec3(0.0, -3.0, 0.0),
		glm::vec3(180.0, 0.0, 0.0),
		glm::vec3(2, 2, -2)
	);
	{ glTFLoader ldr = glTFLoader("assets/models/mary/mary.gltf", m_Model); }
	// Screen Quad
	float rectangleVertices[] =
	{
		// positions   // texture Coords
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,

		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f
	};
	m_VertexArray = new VertexArray();
	m_VertexBuffer = new VertexBuffer(rectangleVertices, sizeof(rectangleVertices));
	
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
}

void PostProcessTest::OnDetach()
{
}

void PostProcessTest::OnEvent(Event& event){
	m_CameraController.OnEvent(event);
}

void PostProcessTest::OnUpdate(Timestep ts)
{
	m_time += ts.GetSeconds();
	m_CameraController.OnUpdate(ts);

	m_PostProcess.FowardPass();
	m_Shader->Bind();
	m_CameraController.GetCamera()->Draw(*m_Shader);
	//m_Shader->SetUniformMat4f("u_ViewProjection", m_CameraController.GetCamera()->GetViewProjectionMatrix());
	m_Model->Draw(*m_Shader);

	m_PostProcess.BackwardPass();
	m_PostProcShader->Bind();
	m_PostProcShader->SetUniform1i("u_Texture", 0);
	Renderer::Draw(*m_VertexArray, *m_VertexBuffer, *m_PostProcShader);
	
	m_PostProcShader->Unbind();
}

void PostProcessTest::OnImGuiRender()
{
	ImGui::Text("Application average\n %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	m_CameraController.OnImGuiRender();
	m_Model->OnImGuiRender();
}
