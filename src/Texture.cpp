#include "Texture.h"
#include "GLError.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	/* flip our texture upside down since OpenGL button-left is 0,0, while img storage is top-left */
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	
	GL_CALL(glGenTextures(1, &m_RendererID));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	/* GL_TEXTURE_MIN_FILTER: How texture will be resample down
	 * GL_TEXTURE_MAG_FILTER: Magnified texture
	 * GL_TEXTURE_WRAP_S: Horizontal Wrap
	 * GL_TEXTURE_WRAP_T: Tileing
	 * We must specify this four Parameter!!!
	*/
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	/* Internal Format: how OpenGL stores the data (GL_RGBA8, should specify 8, homany bits per channel)
	 * Format: of localBuffer
	 * Pixels: you can have it nullptr/0 if not ready at this stage you are allocating GPU Memory
	*/
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GL_CALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}
