#include "glpch.h"
#include "Model.h"

#include <vector>

#include "../GLAbstract/Shader.h"
#include "../GLAbstract/Texture.h"
#include "../GLAbstract/Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm/gtc/matrix_transform.hpp"

namespace GLCore::Utils {
    Model::Model(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        :m_Transform(position, rotation, scale){ }

	void Model::Draw(Shader& shader)
	{
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        Renderer::Clear();

        for (unsigned int i = 0; i < m_Meshes.size(); i++)
            m_Meshes[i]->Draw(shader);
	}
}
