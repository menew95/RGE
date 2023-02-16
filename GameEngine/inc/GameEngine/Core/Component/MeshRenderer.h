#pragma once

#include "GameEngine\Core\Component\Renderer.h"

namespace GameEngine
{
	namespace Core
	{
		class Mesh;
		class Material;
		class MeshFilter;

		class GAME_ENGINE_API MeshRenderer
			: public Renderer, public std::enable_shared_from_this<MeshRenderer>
		{
		public:
			MeshRenderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("MeshRenderer"));
			~MeshRenderer() override;

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

			inline std::vector<std::shared_ptr<Material>> GetMaterials() { return m_Materials; }

			void Link() override;
			void Unlink() override;

		private:
			bool CheckVaild() { return m_Materials.size() > 0; }
			struct PerObjectData
			{
				Math::Matrix _world;
				Math::Matrix _worldInvTranspose;
			};

			PerObjectData _perObject;

			std::vector<std::shared_ptr<Material>> m_Materials;

			std::weak_ptr<MeshFilter> m_MeshFilter;

			bool m_ShadowCasting = true;

			RTTR_ENABLE(Renderer)

			RTTR_REGISTRATION_FRIEND
		};
	}
}
