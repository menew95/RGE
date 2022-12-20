#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class Resource;

		class GAME_ENGINE_API Resources
		{
			DECLARE_SINGLETON_CLASS(Resources)

		public:
			void* Load(const tstring& filePath);

		private:

		};
	}
}
