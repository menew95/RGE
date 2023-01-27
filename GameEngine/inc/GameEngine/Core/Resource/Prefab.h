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
			inline void SetRootGameObject(std::shared_ptr<GameObject>& rootGameObject) { m_RootGameObject = rootGameObject; }

		private:
			uuid m_UUID;
			std::shared_ptr<GameObject> m_RootGameObject;
		};
	}
}