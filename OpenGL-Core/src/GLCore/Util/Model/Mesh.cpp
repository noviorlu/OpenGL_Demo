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
		m_VAO->Bind();
		m_IBO->Bind();

		glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr);

		m_IBO->Unbind();
		m_VAO->Unbind();
	}

	int SubMesh::Seralize(std::vector<float>& SceneData, int retptr)
	{
		int pos = SceneData.size();

		SceneData.push_back(SUBMESH);
		SceneData.push_back(2); // header occupy 2 pixel
		SceneData.push_back(m_Material->m_SeralizeID); // MaterialPtr
		SceneData.push_back(0); // TriangleGroupPtr

		SceneData.push_back(-retptr);
		SceneData.push_back(-retptr);
		SceneData.push_back(-retptr);
		SceneData.push_back(-retptr);

		int triCount = m_Indices.size() / 3;
		SceneData[pos + 3] = SeralizeGroup(SceneData, triCount, pos + 4);

		int GroupPtr = SceneData[pos + 3] + 4;
		for (int i = 0; i < triCount; i++) {
			int offset = i * 3;

			SceneData[GroupPtr] = Triangle::SeralizeTriangle(
				SceneData, 
				m_Vertices[m_Indices[offset]], 
				m_Vertices[m_Indices[offset + 1]], 
				m_Vertices[m_Indices[offset + 2]], 
				GroupPtr+1
			);

			GroupPtr++;
		}

		return pos;
	}

	void Mesh::Draw(Shader& shader)
	{
		shader.SetUniformMat4f("u_Model", m_Transform->GetModelMatrix());

		for (auto& subMesh : m_SubMeshes)
		{
			subMesh->Draw(shader);
		}
	}

	int Mesh::Seralize(std::vector<float>& SceneData, int retptr)
	{
		// LAYOUT:  Type    | OccupySize    | InvTFPtr  | GroupPtr
		//          TFPtr   | 0x80          | 0x80      | 0x80
		// 0. check is type MESH, no extra proc, push loc + 2 into stack
		// 1. check is type TRANSFORM, do transform, push loc + 2 into stack
		// (DISCOVER NEGATIVE, know return to upper Level)
		// 2. check is type GROUP, iterate through group's childs
		// 3. pop if Trf do Trf to Ray then return to upper level
		int pos = SceneData.size();

		SceneData.push_back(MESH); // pos
		SceneData.push_back(2); // pos + 1, occupy 2 pixel
		SceneData.push_back(0); // pos + 2
		SceneData.push_back(0); // pos + 3

		SceneData.push_back(0); // pos + 4
		SceneData.push_back(-retptr); // pos + 5
		SceneData.push_back(-retptr); // pos + 6
		SceneData.push_back(-retptr); // pos + 7

		SceneData[pos + 2] = this->m_Transform->Seralize(SceneData, true, pos + 3);
		SceneData[pos + 3] = SeralizeGroup(SceneData, m_SubMeshes.size(), pos + 4);
		SceneData[pos + 4] = this->m_Transform->Seralize(SceneData, false, pos + 5);

		int GroupPtr = SceneData[pos + 2] + 4;
		for (auto submesh : m_SubMeshes)
		{
			SceneData[GroupPtr] = submesh->Seralize(SceneData, GroupPtr+1);
			GroupPtr++;
		}

		return pos;
		
	}

	Cube::Cube() {
		m_VertexArray = new VertexArray();
		m_VertexBuffer = new VertexBuffer(rectangleVertices, sizeof(rectangleVertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
	}

	void Cube::Draw(Shader& shader) {
		m_VertexArray->Bind();
		m_VertexBuffer->Bind();

		glDrawArrays(GL_TRIANGLES, 0, m_VertexBuffer->GetCount() / m_VertexArray->GetVertexSize());
	}

	Quad::Quad() {
		m_VertexArray = new VertexArray();
		m_VertexBuffer = new VertexBuffer(quadVertices, sizeof(quadVertices));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
	}

	void Quad::Draw(Shader& shader) {
		m_VertexArray->Bind();
		m_VertexBuffer->Bind();

		int val = m_VertexBuffer->GetCount() / m_VertexArray->GetVertexSize();
		glDrawArrays(GL_TRIANGLES, 0, val);
	}
}