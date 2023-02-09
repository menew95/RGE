#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/Component/MeshFilter.h"

#include "GameEngine/Core/Component/Renderer.h"

#include "GameEngine/Core/Resource/Mesh.h"

#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<GameEngine::Core::MeshFilter>("MeshFilter")
	.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>()
	.property("m_pShadredMesh", &GameEngine::Core::MeshFilter::GetSharedMesh, &GameEngine::Core::MeshFilter::SetSharedMesh)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetName"),
		metadata(GameEngine::Core::MetaData::ObjectType, GameEngine::Core::ObjectType::Resource),
		metadata(GameEngine::Core::ObjectType::Resource, GameEngine::Core::ResourceType::Mesh)
	)
	.method("GetName", &GameEngine::Core::Mesh::GetName)
	.method("CheckVaild", &GameEngine::Core::MeshFilter::CheckVaild);
}

namespace GameEngine
{
	namespace Core
	{

		MeshFilter::MeshFilter(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName /*= TEXT("MeshFilter")*/)
			: Component(gameObject, componentName)
		{

		}

		MeshFilter::~MeshFilter()
		{

		}


		void MeshFilter::OnEnable()
		{
			m_pRenderer = GetComponent<Renderer>();

			if (m_pRenderer != nullptr)
			{
				m_pRenderer->Link();
			}
		}

		void MeshFilter::OnDisable()
		{
			if (m_pRenderer != nullptr)
			{
				m_pRenderer->Unlink();
			}
		}

	}
}