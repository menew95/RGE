#pragma once

#include "Common.h"

namespace Graphics
{
	class RenderSystem;
	class PipelineLayout;
	class Resource;

	enum class BufferType
	{
		Mesh,
		Material,

	};

	class ResourceBuffer
	{
	public:
		ResourceBuffer(Graphics::RenderSystem* renderSystem, BufferType type)
		: m_BufferType(type)
		, m_RenderSystem(renderSystem)
		{}

		virtual ~ResourceBuffer() {}

		inline BufferType GetBufferType()
		{
			return m_BufferType;
		}

	protected:
		Graphics::RenderSystem* m_RenderSystem;
		BufferType m_BufferType;
	};
}

