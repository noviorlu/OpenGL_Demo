#include "TestTexture2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Application.h"

namespace test {
    TestTexture2D::TestTexture2D() : 
        m_projMatrix(glm::ortho(0.0f, app.getWidthF(), 0.0f, app.getHeightF(), -1.0f, 1.0f)),
        m_viewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_translationA(200, 200, 0), 
        m_translationB(400, 200, 0)
    {
        /* Data - four vertex */
        float vertexInfos[] = {
             100.0f, 100.0f, 0.0f, 0.0f,// 0
             200.0f, 100.0f, 1.0f, 0.0f,// 1
             200.0f, 200.0f, 1.0f, 1.0f,// 2
             100.0f, 200.0f, 0.0f, 1.0f,// 3
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        /* Blending Propose */
        GL_CALL(glEnable(GL_BLEND));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        /* 4 float per vertex, 4 vertex in total */
        m_vb = std::make_unique<VertexBuffer>(vertexInfos, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        m_VAO = std::make_unique<VertexArray>();
        m_ib = std::make_unique<IndexBuffer>(indices, 6);
        m_VAO->AddBuffer(*m_vb, layout);

        m_shader = std::make_unique<Shader>("assets/shaders/Basic.glsl");
        m_texture = std::make_unique<Texture>("assets/textures/10.jpg");
        
        /* set color */
        GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    }
    TestTexture2D::~TestTexture2D() { 
        GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    }

    void TestTexture2D::OnRender()
    {
        Renderer renderer;
        m_shader->Bind();
        m_texture->Bind();
        m_shader->SetUniform1i("u_Texture", 0);
        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translationA);
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;
            m_shader->Bind();
            m_shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_ib, *m_shader);
        }
        {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_translationB);
            glm::mat4 mvp = m_projMatrix * m_viewMatrix * modelMatrix;
            m_shader->Bind();
            m_shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_ib, *m_shader);
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
