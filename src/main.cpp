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

#include "../tests/TestClearColor.h"

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
    if (!app.initCanvas(800, 600)) return -1;
    app.setResizeCallback(OnResize);
    app.setKeyEventCallback(OnKeyEvent);

    app.initGui();

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

    Renderer renderer;

    /* Blending Propose */
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    
    // can set the test here, no need to be menu, maybe good as command line arguments
    currentTest = testMenu;
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");

    while (app.checkCanvas())
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest) {
            currentTest->OnUpdate(0.0f);
			currentTest->OnRender();
            
			ImGui::Begin("Test");
            if(currentTest != testMenu && ImGui::Button("<-")){
				delete currentTest;
				currentTest = testMenu;
			}
            currentTest->OnImGuiRender();
			ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        app.swapCanvasBuffer();
    }

    app.destoryCanvas();
    app.destoryGui();
    return 0;
}