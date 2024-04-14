#include "glpch.h"
#include "Renderer.h"

namespace GLCore::Utils {

    void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
    {
        shader.Bind();
        va.Bind();
        ib.Bind();

        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}