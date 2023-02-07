#pragma once

#include "GameEngine/Core/Resource/Resource.h"

namespace GameEngine
{
	namespace Core
	{
		class GameObject;

		class GAME_ENGINE_API Prefab : public Resource
		{
		public:
			Prefab(const tstring& typeName = TEXT("Prefab"));
			~Prefab() override;

			inline void SetUUID(uuid uuid) { m_UUID = uuid; }
			void SetRootGameObject(std::shared_ptr<GameObject>& rootGameObject);

			inline std::vector<std::shared_ptr<GameObject>>& GetGameObjectList() { return m_GameObjectList; }

			inline std::shared_ptr<GameObject> GetRootGameObject() { return m_RootGameObject; }

			void RegistGameObject(std::shared_ptr<GameObject>& gameObject);

			std::shared_ptr<GameObject> FindGameObject(const tstring& name);

		protected:
			void AddGameObject(std::shared_ptr<GameObject>& gameObject);

			void Release() override;

		private:
			uuid m_UUID;
			std::shared_ptr<GameObject> m_RootGameObject;
			std::vector<std::shared_ptr<GameObject>> m_GameObjectList;
		};
	}
}