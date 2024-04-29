#pragma once
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace GLCore::Utils {
	class Shader;
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	struct Vertex {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
		//bone indexes which will influence this vertex
		//int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		//float m_Weights[MAX_BONE_INFLUENCE];
	};

	class Mesh {
	public:
		// mesh Data
		std::vector<Vertex>       m_Vertices;
		std::vector<unsigned int> m_Indices;
		std::vector<Texture>      m_Textures;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;

	public:
		Mesh(
			std::vector<Vertex> vertices, 
			std::vector<unsigned int> indices, 
			std::vector<Texture> textures
		);
		~Mesh();
		void Draw(Shader& shader);

	private:
		void SetupMesh();
	};
}