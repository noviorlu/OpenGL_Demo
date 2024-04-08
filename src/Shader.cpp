#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GLError.h"
#include "Shader.h"


Shader::Shader(const std::string& filepath) 
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    GL_CALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GL_CALL(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GL_CALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    /* v means we are passing in a float array 
     * GL_FALSE: correspond if the data is column matrix then no need transpose
    */
    GL_CALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GL_CALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    
    GL_CALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) 
        std::cout << "Warning: uniform [" << name << "] doesn't exist!" << std::endl;
    
    m_UniformLocationCache[name] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        // if find shader on that line
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
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
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " Shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

/* Compile shader, send a string of source code to OpenGL */
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /* OpenGL compile vertex&fragment shader, link together, return unique identifier that can be
     * use to bind shader to main code */
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GL_CALL(glAttachShader(program, vs));
    GL_CALL(glAttachShader(program, fs));
    GL_CALL(glLinkProgram(program));
    GL_CALL(glValidateProgram(program));

    /* Delete the Intermediate Shader, shader already in Program */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
