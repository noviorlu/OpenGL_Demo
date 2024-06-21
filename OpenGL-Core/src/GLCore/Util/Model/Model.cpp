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

#include "imgui.h"

namespace GLCore::Utils {
    Model::Model(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        :m_Transform(position, rotation, scale){ }

    void Model::OnImGuiRender()
    {
        ImGui::Begin("Model");
        m_Transform.OnImGuiRender();

        // Write a checkbox to toggle AutoRotate for the model
        ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
        if(m_AutoRotate)
		{
			// If AutoRotate is enabled, write a slider to control the rotation speed
			ImGui::SliderFloat("Rotation Speed", &m_RotationSpeed, 0.0f, 5.0f);
            
            glm::vec3 rot = m_Transform.GetRotation();
            rot.y += m_RotationSpeed;
            m_Transform.SetRotation(rot);
		}

        // Materials
        if (ImGui::CollapsingHeader("Materials"))
        {
            for (auto& mat : m_MaterialPool)
            {
				ImGui::PushID(mat.first.c_str());
                if (ImGui::CollapsingHeader(mat.first.c_str()))
                {
					mat.second->OnImGuiRender();
				}
				ImGui::PopID();
			}
		}

        ImGui::End();
    }

    void Model::Draw(Shader& shader)
	{
        for (unsigned int i = 0; i < m_Meshes.size(); i++)
            m_Meshes[i]->Draw(shader);
	}
    void Model::ConvertToBlinnPhongMaterial()
    {
        m_MaterialPool.clear();
        for (auto mesh : m_Meshes) {
            for (auto subMesh : mesh->m_SubMeshes) {
                std::string matName = subMesh->m_Material->m_Name+"_BlinnPhong";
                if (m_MaterialPool.find(matName) == m_MaterialPool.end()) {
                    m_MaterialPool[matName] = std::make_shared<BlinnPhongMaterial>(subMesh->m_Material);;
                }
                subMesh->m_Material = m_MaterialPool[matName];
			}
        }
    }
}
