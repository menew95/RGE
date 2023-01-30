#pragma once

#include "Object\Object.h"

namespace GameEngine
{
	namespace Core
	{
		class GameObject;

		class GAME_ENGINE_API Scene
			: public Object
		{
		public:
			Scene(const tstring& _typeName = TEXT("Scene"));

			void Resize(size_t size);
			void AddGameObject(std::shared_ptr<GameObject>& gameObject);
			void AddGameObjects(std::shared_ptr<GameObject>& gameObject);

			inline std::vector<std::shared_ptr<GameObject>>& GetAllGameObject() { return m_GameObjects; }

			GameEngine::Core::GameObject* FindObject(const tstring& name);

		protected:
			std::vector<std::shared_ptr<GameObject>> m_GameObjects;

		};
	}
}