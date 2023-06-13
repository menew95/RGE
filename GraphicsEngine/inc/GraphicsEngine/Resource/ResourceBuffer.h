#pragma once

#include "Common.h"

#include "GraphicsEngine/Export.h"

namespace Graphics
{
	class RenderSystem;
	class PipelineLayout;
	class Resource;

	enum class BufferType
	{
		Mesh,
		Material,
		Camera,
		Light
	};

	class GRAPHICSENGINE_DLL_DECLSPEC ResourceBuffer
	{
	public:
		ResourceBuffer(Graphics::RenderSystem* renderSystem, BufferType type, uuid uuid)
		: m_BufferType(type)
		, m_RenderSystem(renderSystem)
			, m_UUID(uuid)
		{}

		virtual ~ResourceBuffer() {}

		inline BufferType GetBufferType()
		{
			return m_BufferType;
		}

		inline void SetUUID(uuid uuid)
		{
			m_UUID = uuid;
		}

		inline const uuid& GetUUID()
		{
			return m_UUID;
		}

	protected:
		Graphics::RenderSystem* m_RenderSystem;
		BufferType m_BufferType;

		uuid m_UUID;
	};
}

