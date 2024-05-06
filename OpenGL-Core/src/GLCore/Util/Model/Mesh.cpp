#include "glpch.h"
#include "Mesh.h"

#include "../GLAbstract/VertexBufferLayout.h"
#include "../GLAbstract/Renderer.h"

namespace GLCore::Utils {
	SubMesh::SubMesh(
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::shared_ptr<Material> material
	)
	{
		m_Vertices = vertices;
		m_Indices = indices;
		m_Material = material;

		SetupSubMesh();
	}

	void SubMesh::SetupSubMesh()
	{
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
		m_VAO->AddBuffer(*m_VBO, Vertex::Layout());

		m_IBO = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());
	}

	void SubMesh::Draw(Shader& shader)
	{
		m_Material->Draw(shader);

		// draw mesh
		Renderer::Draw(*m_VAO, *m_IBO, shader);

		m_IBO->Unbind();
		m_VAO->Unbind();
	}

	void Mesh::Draw(Shader& shader)
	{
		shader.Bind();
		shader.SetUniformMat4f("model", m_Transform->GetModelMatrix());
		
		for (auto& subMesh : m_SubMeshes)
		{
			subMesh->Draw(shader);
		}

		shader.Unbind();
	}
}