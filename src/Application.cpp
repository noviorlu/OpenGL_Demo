#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GLError.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/gtc/matrix_transform.hpp"
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* SET Opengl version to 3.3, and profile to core */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Glew Initalization Error!" << std::endl;

    /* Print OpenGL version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Data - four vertex */
    float vertexInfos[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,// 0
         0.5f, -0.5f, 1.0f, 0.0f,// 1
         0.5f,  0.5f, 1.0f, 1.0f,// 2
        -0.5f,  0.5f, 0.0f, 1.0f,// 3
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    /* Blending Propose */
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    /* 4 float per vertex, 4 vertex in total */
    VertexBuffer vb(vertexInfos, 4 * 4 * sizeof(float));
    
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 projMatrix = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -1.0f, 1.0f);

    Shader shader("assets/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.0f, 0.3f, 8.0f, 1.0f);
    shader.SetUniformMat4f("u_MVP", projMatrix);
    
    Texture texture("assets/textures/10.jpg");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 8.0f, 1.0f);
        
        renderer.Draw(va, ib, shader);
        
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}