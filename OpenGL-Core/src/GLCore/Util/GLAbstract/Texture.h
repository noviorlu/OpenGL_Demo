#pragma once
#include <string>

#include <glad/glad.h>

namespace GLCore::Utils {

	class Texture {
	public:
		enum class TextureType {
			DIFFUSE,
			SPECULAR,
			NORMAL,
			HEIGHT
		};

		TextureType m_Type;
		std::string m_FilePath;
		unsigned int m_RendererID;
		int m_SeralizeID = -1;
	public:
		Texture();
		Texture(float width, float height);
		Texture(float width, float height, size_t format, size_t dataType, const void* buffer);
		Texture(const std::string& path);
		Texture(const std::string& path, TextureType type);
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