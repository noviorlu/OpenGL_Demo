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

		void Seralize(std::vector<float>& SceneData) {
			SceneData.push_back(Position.x);
			SceneData.push_back(Position.y);
			SceneData.push_back(Position.z);
			SceneData.push_back(TexCoords.x);

			SceneData.push_back(Normal.x);
			SceneData.push_back(Normal.y);
			SceneData.push_back(Normal.z);
			SceneData.push_back(TexCoords.y);
		}
	};

	class Triangle : IIterator {
	public:
		static int SeralizeTriangle(
			std::vector<float>& SceneData,
			Vertex& A, Vertex& B, Vertex& C,
			int retptr) 
		{
			int pos = SceneData.size();

			SceneData.push_back(TRIANGLE);
			SceneData.push_back(7); // header occupy 7 pixel
			SceneData.push_back(-retptr);
			SceneData.push_back(-retptr);

			A.Seralize(SceneData); // 2 pixel
			B.Seralize(SceneData); // 2 pixel
			C.Seralize(SceneData); // 2 pixel

			return pos;
		}
	};

	class SubMesh : IIterator{
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
		int Seralize(std::vector<float>& SceneData, int retptr);

	private:
		void SetupSubMesh();
	};

	class Mesh : IIterator{
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
		int Seralize(std::vector<float>& SceneData, int retptr);
	};

	class Cube {
	public:
		// Cube vertices
		float rectangleVertices[180] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		Cube();
		~Cube() {
			delete m_VertexArray;
			delete m_VertexBuffer;
		}
		void Draw(Shader& shader);
	private:
		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
	};

	class Quad {
	public:
		float quadVertices[24] = {
			// pos         // texUV
			-1.0f,  1.0f,    0.0f, 1.0f, 
			-1.0f, -1.0f,    0.0f, 0.0f,
			 1.0f, -1.0f,    1.0f, 0.0f,

			-1.0f,  1.0f,    0.0f, 1.0f,
			 1.0f, -1.0f,    1.0f, 0.0f,
			 1.0f,  1.0f,    1.0f, 1.0f 
		};

		Quad();
		~Quad() {
			delete m_VertexArray;
			delete m_VertexBuffer;
		}
		void Draw(Shader& shader);
	private:
		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
	};
}