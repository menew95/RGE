#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/Component/SkinnedMeshRenderer.h"
#include "GameEngine/Core/Component/MeshFilter.h"


namespace GameEngine
{
	namespace Core
	{

		SkinnedMeshRenderer::SkinnedMeshRenderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("MeshRenderer")*/)
			: Renderer(gameObject, componentName)
		{

		}

		SkinnedMeshRenderer::~SkinnedMeshRenderer()
		{

		}

		void SkinnedMeshRenderer::Awake()
		{

		}

		void SkinnedMeshRenderer::Render()
		{

		}

		void SkinnedMeshRenderer::Link()
		{

		}

		void SkinnedMeshRenderer::Unlink()
		{

		}

	}
}