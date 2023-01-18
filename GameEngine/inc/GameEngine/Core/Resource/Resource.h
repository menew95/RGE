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

			inline void SetName(const tstring& resourceName) { m_ResourceName = resourceName; }
			inline const tstring& GetName() { return m_ResourceName; }

		protected:
			tstring m_ResourceName;

			virtual void Release() abstract;
		};

	}
}
