#include "glpch.h"
#include "Texture.h"

#include <stb_image.h>

namespace GLCore::Utils {

	Texture::Texture(const std::string& path)
		: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
	{
		/* flip our texture upside down since OpenGL button-left is 0,0, while img storage is top-left */
		stbi_set_flip_vertically_on_load(true);
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

		// Rotate the image 90 degrees counterclockwise
		unsigned char* rotatedBuffer = new unsigned char[m_Width * m_Height * m_BPP];
		for (int y = 0; y < m_Height; ++y) {
			for (int x = 0; x < m_Width; ++x) {
				int index = ((m_Height - y - 1) * m_Width + x) * m_BPP;
				int rotatedIndex = (x * m_Height + y) * m_BPP;
				for (int channel = 0; channel < m_BPP; ++channel) {
					rotatedBuffer[rotatedIndex + channel] = m_LocalBuffer[index + channel];
				}
			}
		}
		// Replace the original buffer with the rotated buffer
		delete[] m_LocalBuffer;
		m_LocalBuffer = rotatedBuffer;
		// Swap width and height
		std::swap(m_Width, m_Height);

		glGenTextures(1, &m_RendererID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		/* GL_TEXTURE_MIN_FILTER: How texture will be resample down
		 * GL_TEXTURE_MAG_FILTER: Magnified texture
		 * GL_TEXTURE_WRAP_S: Horizontal Wrap
		 * GL_TEXTURE_WRAP_T: Tileing
		 * We must specify this four Parameter!!!
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/* Internal Format: how OpenGL stores the data (GL_RGBA8, should specify 8, homany bits per channel)
		 * Format: of localBuffer
		 * Pixels: you can have it nullptr/0 if not ready at this stage you are allocating GPU Memory
		*/
		GLenum format;
		if (m_BPP == 4)
			format = GL_RGBA;
		else if (m_BPP == 3)
			format = GL_RGB;
		else if (m_BPP == 1)
			format = GL_RED;
		else {
			std::cout << "Texture Format not supported" << std::endl;
			return; // Exit or handle the error appropriately
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		/* Cleanup Stage*/
		glBindTexture(GL_TEXTURE_2D, 0);
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	Texture::Texture(const std::string& path, TextureType type)
		: Texture(path)
	{
		m_Type = type;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}