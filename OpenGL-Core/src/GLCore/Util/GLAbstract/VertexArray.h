#pragma once

#include <glad/glad.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GLCore::Utils {

	class VertexArray {
	private:
		unsigned int m_RendererID;
		unsigned int m_VertexSize;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetVertexSize() const { return m_VertexSize; }
	};
}