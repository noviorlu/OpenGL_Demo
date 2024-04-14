#pragma once
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace GLCore::Utils {

	class Shader
	{
	public:
		~Shader();

		GLuint GetRendererID() { return m_RendererID; }

		static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	
		void Bind() const;
		void Unbind() const;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void SetUniform4fv(const std::string& name, const glm::vec4& value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	private:
		Shader() = default;

		void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		GLuint CompileShader(GLenum type, const std::string& source);
		int GetUniformLocation(const std::string& name);
	private:
		GLuint m_RendererID;

		std::string m_FilePath;
		// caching for uniforms
		std::unordered_map<std::string, int> m_UniformLocationCache;
	};

}