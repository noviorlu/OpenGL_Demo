#include "glpch.h"
#include "Mesh.h"

#include "../GLAbstract/VertexBufferLayout.h"
#include "../GLAbstract/Renderer.h"

namespace GLCore::Utils {
	Mesh::Mesh(
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<std::shared_ptr<Texture>> textures,
		Transform* transform
	)
	{
		m_Vertices = vertices;
		m_Indices = indices;
		m_Textures = textures;
		m_Transform = transform;

		SetupMesh();
	}

	void Mesh::SetupMesh()
	{
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
		m_VAO->AddBuffer(*m_VBO, Vertex::Layout());

		m_IBO = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());
	}

	void Mesh::Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		shader.Bind();
		shader.SetUniformMat4f("model", m_Transform->GetModelMatrix());
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->Bind(i);

			std::string name;

			// name here should be match what defined in shader
			switch(m_Textures[i]->m_Type){
				case Texture::TextureType::DIFFUSE:
					name = "texture_diffuse" + std::to_string(diffuseNr++);
					break;
				case Texture::TextureType::SPECULAR:
					name = "texture_specular" + std::to_string(specularNr++);
					break;
				case Texture::TextureType::NORMAL:
					name = "texture_normal" + std::to_string(normalNr++);
					break;
				case Texture::TextureType::HEIGHT:
					name = "texture_height" + std::to_string(heightNr++);
					break;
			}
			
			shader.SetUniform1i(name.c_str(), i);
		}

		// draw mesh
		Renderer::Draw(*m_VAO, *m_IBO, shader);

		shader.Unbind();
		m_IBO->Unbind();
		m_VAO->Unbind();
	}
}