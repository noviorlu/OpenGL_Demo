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

    int Model::SeralizeMaterial(std::vector<float>& MatData, int initialPos) {
        SetupMatTexID();
        
        int frac = m_MaterialPool.size() / 4 + 1;
        for (int i = 0; i < frac * 4; i++) MatData.push_back(0);
        initialPos = 0;

        int pos = MatData.size();
        for (auto mat : m_MaterialPool)
        {
            MatData[initialPos] = mat.second->Seralize(MatData);
            initialPos++;
		}
		return pos;
    }

    int Model::Seralize(std::vector<float>& SceneData, int retptr) {
        // LAYOUT:  Type    | OccupySize    | InvTFPtr  | GroupPtr
        //          TFPtr   | 0x80          | 0x80      | 0x80
        // 0. check is type MODEL, no extra proc, push loc + 2 into stack
        // 1. check is type TRANSFORM, do transform, push loc + 2 into stack
        // (DISCOVER NEGATIVE, know return to upper Level)
        // 2. check is type GROUP, iterate through group's childs
        // 3. pop if Trf do Trf to Ray then return to upper level
        int pos = SceneData.size();
        
        SceneData.push_back(MODEL); // pos
        SceneData.push_back(2); // pos + 1, header occupy 2 pixel
        SceneData.push_back(0); // pos + 2
        SceneData.push_back(0); // pos + 3

        SceneData.push_back(0); // pos + 4
        SceneData.push_back(-retptr); // pos + 5
        SceneData.push_back(-retptr); // pos + 6
        SceneData.push_back(-retptr); // pos + 7

        SceneData[pos + 2] = this->m_Transform.Seralize(SceneData, true, pos + 3);
        SceneData[pos + 3] = SeralizeGroup(SceneData, m_Meshes.size(), pos + 4);
        SceneData[pos + 4] = this->m_Transform.Seralize(SceneData, false, pos + 5);

        int GroupPtr = SceneData[pos + 3] + 4;
        for (auto mesh : m_Meshes)
        {
			SceneData[GroupPtr] = mesh->Seralize(SceneData, GroupPtr+1);
            GroupPtr++;
		}

        return pos;
    }
}
