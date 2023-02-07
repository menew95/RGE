#pragma once

#include "Common.h"

namespace Utility
{
	struct PrefabData;
	struct AnimationClipData;
}

namespace GameEngine
{
	namespace Core
	{
		class GameObject;
		class Resource;
		class Mesh;
		class Material;
		class AnimationClip;

		class Prefab;


		class GAME_ENGINE_API Resources
		{
			DECLARE_SINGLETON_CLASS(Resources)

		public:
			void Init();

			void* Load(const tstring& filePath);

			void LoadFBX(const tstring& filePath);

			void CreatePrefab(const Utility::PrefabData& prefabData);

			template<typename T>
			std::shared_ptr<T> GetResource(uuid uuid)
			{
				static_assert(false, "need template specialization for T");
			}

			template<>
			std::shared_ptr<Mesh> GetResource(uuid uuid);

			template<>
			std::shared_ptr<Material> GetResource(uuid uuid);

			template<>
			std::shared_ptr<AnimationClip> GetResource(uuid uuid);

			template<>
			std::shared_ptr<Prefab> GetResource(uuid uuid);


			std::shared_ptr<Mesh>& GetMesh(uuid uuid);
			std::shared_ptr<Material>& GetMaterial(uuid uuid);
			std::shared_ptr<AnimationClip>& GetAnimationClip(uuid uuid);

			std::shared_ptr<Prefab>& GetPrefab(uuid uuid);

			bool LoadMaterial(uuid uuid);
			bool LoadMesh(uuid uuid);
			bool LoadAnimation(uuid uuid);
			bool LoadPrefab(uuid uuid);

		private:
			void CreateAnimationClip(Utility::AnimationClipData* clipData);

			std::unordered_map<uuid, std::shared_ptr<Mesh>> m_MeshMap;
			std::unordered_map<uuid, std::shared_ptr<Material>> m_MaterialMap;
			std::unordered_map<uuid, std::shared_ptr<AnimationClip>> m_AnimationClipMap;

			std::unordered_map<uuid, std::shared_ptr<Prefab>> m_PrefabMap;
		};
	}
}
