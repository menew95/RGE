#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class GameObject;
		class Resource;
		class Mesh;
		class Material;

		class GAME_ENGINE_API Resources
		{
			DECLARE_SINGLETON_CLASS(Resources)

		public:
			void Init();

			void* Load(const tstring& filePath);

			void LoadFBX(const tstring& filePath);

			std::shared_ptr<Mesh>& GetMesh(uuid uuid);
			std::shared_ptr<Material>& GetMaterial(uuid uuid);

		private:
			std::shared_ptr<Mesh> m_NullMesh = nullptr;
			std::shared_ptr<Material> m_NullMaterial = nullptr;

			std::unordered_map<uuid, std::shared_ptr<Mesh>> m_MeshMap;
			std::unordered_map<uuid, std::shared_ptr<Material>> m_MaterialMap;
		};
	}
}
