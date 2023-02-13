#pragma once

#include "Common.h"

#include "GraphicsModule/Core/ResourceFlags.h"

#include "GraphicsModule/Core/CommandBufferFlags.h"

namespace Graphics
{
	class PipelineState;
	class PipelineLayout;
	class RenderTarget;

	struct ResourceClear
	{
		ResourceType _type;
		uint32 _fristSlot;
		uint32 _numSlots;
		long _bindFlags;
		long _stageFlags;
	};

	struct RenderPassDesc
	{
		tstring							_passName = TEXT("Render Pass");

		PipelineState*					_pipelineState	= nullptr;
		PipelineLayout*					_pipelineLayout	= nullptr;
		RenderTarget*					_renderTarget	= nullptr;

		Buffer*							_perFrameBuffer	= nullptr;

		std::vector<AttachmentClear>	_attachmentClears;

		bool							_IsClearObjects = true;

		std::vector<ResourceClear>		_resourceClears;
	};
}