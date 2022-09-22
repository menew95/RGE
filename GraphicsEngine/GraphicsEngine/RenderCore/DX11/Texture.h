#pragma once

#include "IResource.h"

namespace RenderCore
{
	class Texture : public IResource<Texture>, public std::enable_shared_from_this<Texture>
	{
	public:
		Texture();
		
		~Texture();

	};
}