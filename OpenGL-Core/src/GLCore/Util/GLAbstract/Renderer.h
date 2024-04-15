#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"

namespace GLCore::Utils {
	class Renderer 
	{
	public:
		static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
		static void Draw(const VertexArray& va, const VertexBuffer& vb, const Shader& shader);
		static void Clear();
	};
}