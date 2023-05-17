#include "GraphicsModule/Core/ResourceFlags.h"
#include "GraphicsModule/Core/TextureFlags.h"

namespace Graphics
{
	class Resource;

	struct ResourceViewDesc
	{
		std::vector<Resource*> _resources;
		std::vector<TextureSubresource> _texSubresources;
	};
}