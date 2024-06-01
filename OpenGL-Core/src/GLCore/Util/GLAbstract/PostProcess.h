#pragma once
#include <string>

#include <glad/glad.h>

#include "Texture.h"
#include "../../Core/Application.h"

namespace GLCore::Utils {

	class PostProcess {
	public:
		unsigned int m_fbo;
		unsigned int m_rbo;
		Texture* m_TextureColor;
		int m_Width, m_Height;
	public:
		PostProcess() {
			m_Width = Application::Get().GetWindow().GetWidth();
			m_Height = Application::Get().GetWindow().GetHeight();
			
			// configure framebuffer
			glGenFramebuffers(1, &m_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			
			// create a color attachment texture
			m_TextureColor = new Texture(m_Width, m_Height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColor->m_RendererID, 0);
		
			// configure depth and stencil renderbuffer
			glGenRenderbuffers(1, &m_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
		
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		~PostProcess() {
			glDeleteFramebuffers(1, &m_fbo);
		}
	};
}