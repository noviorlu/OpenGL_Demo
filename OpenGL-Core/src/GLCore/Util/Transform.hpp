#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

namespace GLCore::Utils {
	class Transform
	{
	public:
		Transform() {}
		Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: m_Translation(translation), m_Rotation(glm::quat(rotation)), m_Scale(scale) { }
		Transform(
			const glm::vec3& translation, 
			const glm::vec3& rotation,
			const glm::vec3& scale, 
			Transform* parent
		)
			: Transform(translation, rotation, scale)
		{
			SetParent(parent);
		}
		// ReadOnly Transform
		Transform(
			const glm::mat4& matrix, Transform* parent
		) {
			readOnly = true;
			isDirty = false;
			m_Matrix = matrix;
		}

		~Transform(){
			for(auto child : m_Childrens)
				delete child;
		}

		Transform* GetParent() { return m_Parent; }
		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::vec3& GetRotation() const { return glm::eulerAngles(m_Rotation); }
		const glm::vec3& GetScale() const { return m_Scale; }

		void SetParent(Transform* parent)
		{
			if (m_Parent == parent) return;
			if(m_Parent != nullptr)
				m_Parent->RemoveChild(this);

			m_Parent = parent;
			m_Parent->AddChild(this);
		}

		void AddChild(Transform* child)
		{
			m_Childrens.push_back(child);
		}

		void RemoveChild(Transform* child)
		{
			auto it = std::find(m_Childrens.begin(), m_Childrens.end(), child);
			if (it != m_Childrens.end())
				m_Childrens.erase(it);
		}

		void SetRotation(const glm::vec3& rotation)
		{
			m_Rotation = glm::quat(rotation);
			isDirty = true;
		}

		void SetTranslation(const glm::vec3& translation)
		{
			m_Translation = translation;
			isDirty = true;
		}

		void SetScale(const glm::vec3& scale)
		{
			m_Scale = scale;
			isDirty = true;
		}

		const glm::mat4& GetModelMatrix() {
			if (readOnly) {
				return m_Parent->GetModelMatrix() * m_Matrix;
			}
			if(isDirty)
			{
				UpdateModelMatrix();
				isDirty = false;
			}
			return m_Matrix;
		}

        		
	private:
		Transform* m_Parent = nullptr;
		std::vector<Transform*> m_Childrens;

		glm::vec3 m_Translation = glm::vec3(0.0, 0.0, 0.0);
		glm::quat m_Rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
		glm::vec3 m_Scale = glm::vec3(1.0, 1.0, 1.0);

		glm::mat4 m_Matrix = glm::mat4(1.0f);

		bool isDirty = true;
		bool readOnly = false;

		void setDirty()
		{
			isDirty = true;
			for(auto child : m_Childrens)
			{
				child->setDirty();
			}
		}

		void UpdateModelMatrix()
		{
			if(readOnly)
				assert(false);

			if (m_Parent)
				m_Matrix = m_Parent->GetModelMatrix();
			else
				m_Matrix = glm::mat4(1.0f);

			m_Matrix =
				glm::translate(m_Matrix, m_Translation) *
				glm::toMat4(m_Rotation) *
				glm::scale(glm::mat4(1.0f), m_Scale);
		}
	};
}