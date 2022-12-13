#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class Resource;

		class Resources
		{
			DECLARE_SINGLETON_CLASS(Resources)

		public:
			void* Load(const tstring& filePath);

		private:

		};
	}
}
