#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace GLCore::Utils {
	class Renderer 
	{
	public:
		static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
		static void Clear();
	};
}