#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); /* same as &source[0] */

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    /* Error handling */
    int result;
    /* i specify we want a integer, v means vector/array which here means it want's a pointer*/
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        /* alloca: allocate memory on stack */
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex":"fragment") 
            << " Shader!" << std::endl;
        std::cout << message << std::endl;
        
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/* Compile shader, send a string of source code to OpenGL */ 
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /* OpenGL compile vertex&fragment shader, link together, return unique identifier that can be 
     * use to bind shader to main code */
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    /* Delete the Intermediate Shader, shader already in Program */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Glew Initalization Error!" << std::endl;

    /* Print OpenGL version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Data - three vertex position*/
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer; /* PointerORindex to generated buffer */
    glGenBuffers(1, &buffer);
    /* Define propose of the buffer, Select==Bind this buffer into the stateMachine */
    glBindBuffer(GL_ARRAY_BUFFER, buffer); 
    /* Define buffer Size, pass in prepared data */
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);   
    
    /* Vertex Attribute Setup */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Specify how OpenGL interperts the data (3 vertices, float, 2 float represent 1 vertex, etc.) */
        /* Generally: How our Data is Layed out?*/
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteShader(shader);

    glfwTerminate();
    return 0;
}