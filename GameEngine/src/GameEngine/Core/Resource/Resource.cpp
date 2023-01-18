#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Resource\Resource.h"

namespace GameEngine
{
	namespace Core
	{

		Resource::Resource(const tstring& typeName /*= TEXT("Resource")*/)
			: Object(typeName)
		{

		}

		Resource::~Resource()
		{

		}
	}
}