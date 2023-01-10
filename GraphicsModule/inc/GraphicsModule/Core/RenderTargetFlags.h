#pragma once

#include "Math/Math.h"

namespace Graphics
{
	enum class RenderTargetType
	{
		RenderTarget = 1,
		Depth = 2,
		DepthStencil = 3,
		Stencil = 4,
	};

	struct AttachmentDesc
	{
		RenderTargetType _renderTargetTyep = RenderTargetType::RenderTarget;

		class Texture* _resource = nullptr;

		uint32 _mipLevel = 0;

		uint32 _arrayLayer = 0;
	};

	struct RenderTargetDesc
	{
		Extend2D _extend;

		uint32 _sample = 1;

		std::vector<AttachmentDesc> _attachments;
	};
}