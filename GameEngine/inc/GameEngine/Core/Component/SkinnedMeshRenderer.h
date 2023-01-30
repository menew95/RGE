#pragma once

#include "GameEngine/Core/Component/Renderer.h"

namespace GameEngine
{
	namespace Core
	{
		class Mesh;
		class Material;
		class MeshFilter;

		class GAME_ENGINE_API SkinnedMeshRenderer
			: public Renderer, public std::enable_shared_from_this<SkinnedMeshRenderer>
		{
		public:
			SkinnedMeshRenderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("MeshRenderer"));
			~SkinnedMeshRenderer() override;

			void Awake() override;

			void Render() override;

			inline void AddMaterial(std::shared_ptr<Material>& material)
			{
				m_Materials.push_back(material);
			}

			inline void SetMaterial(uint32 i, std::shared_ptr<Material>& material)
			{
				if (i < static_cast<uint32>(m_Materials.size()))
				{
					m_Materials[i] = material;
				}
			}

			inline void SetMaterials(std::vector<std::shared_ptr<Material>>& materials) { m_Materials = materials; }

			inline std::shared_ptr<Material>& GetMaterial(uint32 i) { return m_Materials[i]; }

			inline auto GetMaterials() { return m_Materials; }

			void Link() override;
			void Unlink() override;

			inline void AddBone(std::weak_ptr<Transform> bone, Math::Matrix offset)
			{ 
				m_BoneTransformList.push_back(bone);
				m_BoneOffset.push_back(offset);
			}

		private:
			void UpdateBoneTransform();

			struct PerObjectData
			{
				Math::Matrix _world;
				Math::Matrix _worldInvTranspose;
			};

			struct PerSkinnedObjectData
			{
				Math::Matrix _boneTransform[128];
			};

			PerObjectData _perObject;
			PerSkinnedObjectData _perSkinnedObject;

			std::vector<std::shared_ptr<Material>> m_Materials;

			std::weak_ptr<MeshFilter> m_MeshFilter;

			std::vector<std::weak_ptr<Transform>> m_BoneTransformList;
			std::vector<Math::Matrix> m_BoneOffset;
		};
	}
}
