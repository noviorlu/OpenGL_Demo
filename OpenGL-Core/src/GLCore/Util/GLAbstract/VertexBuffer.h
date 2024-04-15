#pragma once

#include <glad/glad.h>

namespace GLCore::Utils {

	class VertexBuffer {
	private:
		unsigned int m_rendererID;
		unsigned int m_Count;
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; };
	};
}