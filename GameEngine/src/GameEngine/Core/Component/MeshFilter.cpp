#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/Component/MeshFilter.h"

#include "GameEngine/Core/Component/Renderer.h"

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
			m_pRenderer->Link();
		}

		void MeshFilter::OnDisable()
		{
			m_pRenderer->Unlink();
		}

	}
}