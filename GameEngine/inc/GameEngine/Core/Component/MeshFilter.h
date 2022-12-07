#pragma once

#include "Common.h"
#include "GameEngine\Core\Component\Component.h"

namespace GameEngine
{
	namespace Core
	{
		class MeshFilter
			: public Component, public std::enable_shared_from_this<MeshFilter>
		{
		public:
			MeshFilter(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName = TEXT("MeshFilter"));
			virtual ~MeshFilter();

			std::shared_ptr<class Mesh>& GetSharedMesh() { return m_pSharedMesh; }

		private:
			std::shared_ptr<class Mesh> m_pSharedMesh;
		};
	}
}