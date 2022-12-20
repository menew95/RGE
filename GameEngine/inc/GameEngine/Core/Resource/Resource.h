#pragma once

#include "Common.h"
#include "Object\Object.h"

namespace GameEngine
{
	namespace Core
	{
		class GAME_ENGINE_API Resource
			: public Object
		{
		public:
			Resource(const tstring& typeName = TEXT("Resource"));
			virtual ~Resource();

		protected:
			virtual bool Release() abstract;
		};

	}
}
