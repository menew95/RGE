#pragma once

#include "Object\Object.h"

namespace GameEngine
{
	namespace Metal
	{
		class Scene 
			: public Object
		{
		public:
			Scene(const tstring& _typeName = TEXT("Scene"));


		protected:
			std::vector<class Component*> m_
		};
	}
}