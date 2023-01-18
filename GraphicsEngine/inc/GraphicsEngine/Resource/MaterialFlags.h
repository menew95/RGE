#pragma once

#include "Common.h"

#include "GraphicsModule/Core/ResourceFlags.h"

namespace Graphics
{

	struct ResourceDesc
	{
		tstring _resourceUUID;
		ResourceType _type;
	};

	struct MaterialDesc
	{
		tstring _pipelineLayout;

		std::vector<tstring> _resource;
	};
}