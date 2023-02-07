#pragma once

#include "Common.h"
#include "GameEngine\Core\Component\Component.h"

namespace GameEngine
{
	namespace Core
	{
		class Mesh;
		class GAME_ENGINE_API MeshFilter
			: public Component, public std::enable_shared_from_this<MeshFilter>
		{
		public:
			MeshFilter(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName = TEXT("MeshFilter"));
			virtual ~MeshFilter();

			inline void SetSharedMesh(std::shared_ptr<Mesh>& _sharedMesh) { m_pSharedMesh = _sharedMesh; }
			inline std::shared_ptr<Mesh>& GetSharedMesh() { return m_pSharedMesh; }

			void OnEnable() override;
			void OnDisable() override;

		private:
			bool CheckVaild() { return m_pSharedMesh != nullptr; }
			std::shared_ptr<class Renderer> m_pRenderer;

			std::shared_ptr<class Mesh> m_pSharedMesh;

			RTTR_ENABLE(Component)

			RTTR_REGISTRATION_FRIEND
		};
	}
}