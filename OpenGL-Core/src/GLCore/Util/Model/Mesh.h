#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../GLAbstract/VertexArray.h"
#include "../GLAbstract/VertexBuffer.h"
#include "../GLAbstract/VertexBufferLayout.h"
#include "../GLAbstract/IndexBuffer.h"
#include "../GLAbstract/Texture.h"

#include "Material.h"
#include "../Transform.hpp"


namespace GLCore::Utils {
	struct Vertex {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		//glm::vec3 Tangent;
		//glm::vec3 Bitangent;
		//int m_BoneIDs[MAX_BONE_INFLUENCE];
		//float m_Weights[MAX_BONE_INFLUENCE];

		static VertexBufferLayout Layout() {
			VertexBufferLayout layout;
			layout.Push<float>(3); // position
			layout.Push<float>(3); // normal
			layout.Push<float>(2); // texcoords
			return layout;
		}
	};

	class SubMesh {
	public:
		std::vector<Vertex>       m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::shared_ptr<Material> m_Material;

		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<VertexBuffer>	m_VBO;
		std::unique_ptr<IndexBuffer>	m_IBO;

	public:
		SubMesh(
			std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::shared_ptr<Material> material
		);
		~SubMesh() {
			m_VAO->Unbind();
			m_VBO->Unbind();
			m_IBO->Unbind();
		}
		void Draw(Shader& shader);

	private:
		void SetupSubMesh();
	};

	class Mesh {
	public:
		std::string m_Name;
		Transform* m_Transform;

		std::vector<std::shared_ptr<SubMesh>> m_SubMeshes;

	public:
		Mesh() {}
		Mesh(const std::string& name, Transform* transform)
		: m_Name(name), m_Transform(transform) {}
		~Mesh() {
			m_SubMeshes.clear();
		}
		void Draw(Shader& shader);
	};

	class Quad {
	public:
		// Cube vertices
		float rectangleVertices[24] =
		{
			// positions   // texture Coords
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 1.0f,

			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f
		};

		Quad() {
			m_VertexArray = new VertexArray();
			m_VertexBuffer = new VertexBuffer(rectangleVertices, sizeof(rectangleVertices));

			VertexBufferLayout layout;
			layout.Push<float>(2);
			layout.Push<float>(2);

			m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		}

		void Draw(Shader& shader) {
			m_VertexArray->Bind();
			m_VertexBuffer->Bind();

			glDrawArrays(GL_TRIANGLES, 0, m_VertexBuffer->GetCount() / m_VertexArray->GetVertexSize());
		}
	private:
		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
	};
}