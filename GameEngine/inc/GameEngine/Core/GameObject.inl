#include "GameEngine\Core\GameObject.h"

namespace GameEngine
{
	namespace Core
	{
		template<typename T>
		T* GameEngine::Core::GameObject::AddComponent()
		{
			T* _newComponent = new T(this);

			m_Components.emplace_back(_newComponent);

			return _newComponent;
		}

		template<typename T>
		T* GameEngine::Core::GameObject::GetComponent()
		{
			for (T* component : m_Components)
			{
				const type_info& _info = typeid(*component);
				const type_info& _infoT = typeid(T);

				if (typeid(*component) == typeid(T))
				{
					return static_cast<T*>(component);
				}
			}

			return nullptr;
		}

		template<typename T>
		T* GameEngine::Core::GameObject::GetComponentInChildren()
		{
			// Todo : 추가 필요
			return nullptr;
		}

		template<typename T>
		T* GameEngine::Core::GameObject::GetComponentInParent()
		{
			// Todo : 추가 필요
			return nullptr;
		}

		template<typename T>
		std::vector<T*>
			GameEngine::Core::GameObject::GetComponentsInChildren()
		{
			// Todo : 추가 필요
			return nullptr;
		}

		template<typename T>
		std::vector<T*>
			GameEngine::Core::GameObject::GetComponentsInParent()
		{
			// Todo : 추가 필요
			return nullptr;
		}
	}
}