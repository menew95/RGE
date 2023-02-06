#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Resource\Resource.h"

#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	registration::class_<GameEngine::Core::Resource>("Resource")
	.property("m_ResourceName", &GameEngine::Core::Resource::GetName, &GameEngine::Core::Resource::SetName);
}

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