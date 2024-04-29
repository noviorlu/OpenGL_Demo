#include "glpch.h"
#include "Mesh.h"

#include "../GLAbstract/Shader.h"
#include "../GLAbstract/VertexArray.h"
#include "../GLAbstract/VertexBuffer.h"
#include "../GLAbstract/VertexBufferLayout.h"
#include "../GLAbstract/IndexBuffer.h"
#include "../GLAbstract/Texture.h"
#include "../GLAbstract/Renderer.h"

namespace GLCore::Utils {
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		m_Vertices = vertices;
		m_Indices = indices;
		m_Textures = textures;

		SetupMesh();
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::SetupMesh()
	{
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));

		VertexBufferLayout layout;
		layout.Push<float>(3); // position
		layout.Push<float>(3); // normal
		layout.Push<float>(2); // texcoords
		layout.Push<float>(3); // tangent
		layout.Push<float>(3); // bitangent
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());
	}

	void Mesh::Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		shader.Bind();
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i].Bind(i);

			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name;

			if(m_Textures[i].m_Type == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if(m_Textures[i].m_Type == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if(m_Textures[i].m_Type == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if(m_Textures[i].m_Type == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

			// now set the sampler to the correct texture unit
			shader.SetUniform1i((name + number).c_str(), i);
		}

		// draw mesh
		Renderer::Draw(*m_VAO, *m_IBO, shader);

		shader.Unbind();
		m_IBO->Unbind();
		m_VAO->Unbind();
	}
}