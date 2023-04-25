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
			SkinnedMeshRenderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("SkinnedMeshRenderer"));
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

			inline void SetMaterials(std::vector<std::shared_ptr<Material>> materials) { m_Materials = materials; }

			inline std::shared_ptr<Material>& GetMaterial(uint32 i) { return m_Materials[i]; }

			inline std::vector<std::shared_ptr<Material>> GetMaterials() 
			{
				return m_Materials; 
			}

			void Link() override;
			void Unlink() override;

			inline void AddBone(std::weak_ptr<Transform> bone, Math::Matrix offset)
			{ 
				m_BoneTransformList.push_back(bone);
				m_BoneOffset.push_back(offset);
			}

			inline std::shared_ptr<Transform> GetRootBone()
			{
				return m_RootBone;
			}

			inline void SetRootBone(std::shared_ptr<Transform> root)
			{
				m_RootBone = root;
			}

		private:
			bool CheckVaild() { return m_Materials.size() > 0; }
			bool CheckRootVaild() { return m_RootBone != nullptr; }

			void UpdateBoneTransform();

			struct PerSkinnedObjectData
			{
				Math::Matrix _boneTransform[128];
			};

			PerSkinnedObjectData _perSkinnedObject;

			std::vector<std::shared_ptr<Material>> m_Materials;

			std::weak_ptr<MeshFilter> m_MeshFilter;

			std::shared_ptr<Transform> m_RootBone;

			std::vector<std::weak_ptr<Transform>> m_BoneTransformList;
			std::vector<Math::Matrix> m_BoneOffset;

			RTTR_ENABLE(Renderer)

			RTTR_REGISTRATION_FRIEND
		};
	}
}
