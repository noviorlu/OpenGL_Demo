#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "GLError.h"
#include "Application.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/gtc/matrix_transform.hpp"

Shader* m_shader = nullptr;

void OnResize(int width, int height) {
    //std::cout << "Window Size:" << width << ":" << height << std::endl;
    GL_CALL(glViewport(0, 0, width, height));
}

void OnKeyEvent(int key, int scancode, int action, int mods) {
    std::cout << "KeyEvent: " << key << "; action: " << action << "; mods: " << mods << std::endl;
}

int main(void)
{
    if (!app.init(800, 600)) return -1;
    app.setResizeCallback(OnResize);
    app.setKeyEventCallback(OnKeyEvent);

    /* Print OpenGL version */
    std::cout << glGetString(GL_VERSION) << std::endl;

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

    VertexArray va;
    /* 4 float per vertex, 4 vertex in total */
    VertexBuffer vb(vertexInfos, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 projMatrix = glm::ortho(0.0f, app.getWidthF(), 0.0f, app.getHeightF(), -1.0f, 1.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    //glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
    //glm::mat4 mvp = projMatrix * viewMatrix * modelMatrix;

    m_shader = new Shader("assets/shaders/Basic.glsl");
    Texture texture("assets/textures/10.jpg");
    m_shader->Bind();
    texture.Bind();
    m_shader->SetUniform1i("u_Texture", 0);
    
    va.Unbind();
    m_shader->Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    /* [OpenGL][GeneralSetup]: clearviewport and color */
    GL_CALL(glViewport(0, 0, app.getWidth(), app.getHeight()));
    GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    float r = 0.0f;
    float increment = 0.05f;
    /* [GLFW] Window while loop */
    while (true)
    {
        renderer.Clear();
        
        /* Render here */
        if (!app.draw()) break;

        /* Shader setting */
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), app.translation);
        glm::mat4 mvp = projMatrix * viewMatrix * modelMatrix;
        
        m_shader->Bind();
        m_shader->SetUniformMat4f("u_MVP", mvp);
        //shader.SetUniform4f("u_Color", r, 0.3f, 8.0f, 1.0f);
        //if (r > 1.0f)
        //    increment = -0.05f;
        //else if (r < 0.0f)
        //    increment = 0.05f;
        //r += increment;
        

        renderer.Draw(va, ib, *m_shader);
        app.swapBuffer();
    }

    app.destory();
    return 0;
}