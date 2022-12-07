#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"


namespace GameEngine
{
	namespace Core
	{
		template<typename T>
		std::shared_ptr<T>& GameEngine::Core::GameObject::AddComponent()
		{
			std::shared_ptr<T> _newComponent = std::make_shared<T>(this);

			m_Components.emplace_back(_newComponent);

			return m_Components.back();
		}

		template<typename T>
		std::shared_ptr<T>& GameEngine::Core::GameObject::GetComponent()
		{
			for (auto& component : m_Components)
			{
				if (typeid(*component).name() == typeid(T).name())
				{
					return std::static_pointer_cast<T>(component);
				}
			}

			return nullptr;
		}

		template<typename T>
		std::shared_ptr<T>& GameEngine::Core::GameObject::GetComponentInChildren()
		{
			// Todo : 추가 필요

			return nullptr;
		}

		template<typename T>
		std::shared_ptr<T>& GameEngine::Core::GameObject::GetComponentInParent()
		{
			// Todo : 추가 필요

			return nullptr;
		}

		template<typename T>
		std::vector<std::shared_ptr<T>>&
			GameEngine::Core::GameObject::GetComponentsInChildren()
		{
			// Todo : 추가 필요

			return nullptr;
		}

		template<typename T>
		std::vector<std::shared_ptr<T>>&
			GameEngine::Core::GameObject::GetComponentsInParent()
		{
			// Todo : 추가 필요

			return nullptr;
		}
	}
}