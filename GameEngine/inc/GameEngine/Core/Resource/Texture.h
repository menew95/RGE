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
		class Texture
			: public Resource, public std::enable_shared_from_this<Texture>
		{
		public:
			Texture(const tstring& _typeName = TEXT("Texture"));
			virtual ~Texture();

		protected:

		private:
			std::shared_ptr<Graphics::TextureBuffer> m_TextureBuffer;
		};
	}
}