#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"


namespace GameEngine
{
	namespace Core
	{
		template<typename T>
		inline std::shared_ptr<T> GameEngine::Core::GameObject::AddComponent()
		{
			auto _this = shared_from_this();

			std::shared_ptr<T> _newComponent = std::make_shared<T>(_this);

			m_Components.emplace_back(_newComponent);

			return _newComponent;
		}

		template<typename T>
		inline std::shared_ptr<T> GameEngine::Core::GameObject::GetComponent()
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
		inline std::shared_ptr<T> GameEngine::Core::GameObject::GetComponentInChildren()
		{
			// Todo : 추가 필요

			return nullptr;
		}

		template<typename T>
		inline std::shared_ptr<T> GameEngine::Core::GameObject::GetComponentInParent()
		{
			// Todo : 추가 필요

			return nullptr;
		}

		template<typename T>
		inline std::vector<std::shared_ptr<T>>
			GameEngine::Core::GameObject::GetComponentsInChildren()
		{
			// Todo : 추가 필요

			return nullptr;
		}

		template<typename T>
		inline std::vector<std::shared_ptr<T>>
			GameEngine::Core::GameObject::GetComponentsInParent()
		{
			// Todo : 추가 필요

			return nullptr;
		}
	}
}