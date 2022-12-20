#pragma once

#include "Common.h"

#include "GameEngine\Core\Resource\Resource.h"

namespace Graphics
{
	class TextureBuffer;
}

namespace GameEngine
{
	namespace Core
	{
		class GAME_ENGINE_API Sprite
			: public Resource, public std::enable_shared_from_this<Sprite>
		{
		public:
			Sprite(const tstring& _typeName = TEXT("Sprite"));
			virtual ~Sprite();

		protected:

		private:
		};
	}
}