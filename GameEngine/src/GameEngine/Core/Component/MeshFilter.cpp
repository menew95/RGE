#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Component\MeshFilter.h"

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

	}
}