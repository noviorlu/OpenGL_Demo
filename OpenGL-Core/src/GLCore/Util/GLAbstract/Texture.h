#pragma once
#include <string>

#include <glad/glad.h>

namespace GLCore::Utils {

	class Texture {
	public:
		std::string m_Type;
		unsigned int m_RendererID;
		std::string m_FilePath;

	public:
		Texture(const std::string& path);
		Texture(const std::string& path, const std::string& type);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
	
	private:
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	};
}