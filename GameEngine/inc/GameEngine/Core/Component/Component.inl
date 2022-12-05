#include "GameEngine\Core\Component\Component.h"

namespace GameEngine
{
	namespace Core
	{
		template<typename T>
		std::vector<T*>
			Component::GetComponentsInParent()
		{
			if (m_pGameObject != nullptr)
			{
				return m_pGameObject->template GetComponentsInParent<T>();
			}

			return nullptr;
		}

		template<typename T>
		std::vector<T*>
			Component::GetComponentsInChildren()
		{
			if (m_pGameObject != nullptr)
			{
				return m_pGameObject->template GetComponentsInChildren<T>();
			}

			return nullptr;
		}

		template<typename T>
		T* Component::GetComponentInParent()
		{
			if (m_pGameObject != nullptr)
			{
				return m_pGameObject->template GetComponentInParent<T>();
			}

			return nullptr;
		}

		template<typename T>
		T* Component::GetComponentInChildren()
		{
			if (m_pGameObject != nullptr)
			{
				return m_pGameObject->template GetComponentInChildren<T>();
			}

			return nullptr;
		}

		template<typename T>
		T* Component::GetComponent()
		{
			if (m_pGameObject != nullptr)
			{
				return m_pGameObject->template GetComponent<T>();
			}

			return nullptr;
		}

		template<typename T>
		T* Component::AddComponent()
		{
			if (m_pGameObject != nullptr)
			{
				return m_pGameObject->template AddComponent<T>();
			}

			return nullptr;
		}
	}
}