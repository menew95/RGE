#pragma once

#include "IResource.h"

namespace RenderCore
{
	class MeshBuffer : public IResource<MeshBuffer>, public std::enable_shared_from_this<MeshBuffer>
	{

	};
}