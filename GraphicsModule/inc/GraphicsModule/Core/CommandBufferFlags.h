#pragma once

#include "Common.h"
#include "Math/Math.h"

namespace Graphics
{
	// 클리어 정보
	struct ClearValue
	{
		ClearValue() = default;
		ClearValue(const ClearValue&) = default;
		ClearValue& operator=(const ClearValue&) = default;

		inline ClearValue(const Math::Color& color, float depth = 1.f, uint32 stencil = 0)
			: _color{ color }
			, _depth{ depth }
			, _stencil{ stencil }
		{}

		inline ClearValue(float depth)
			: _depth{ depth }
		{}

		inline ClearValue(float depth, uint32 stencil)
			: _depth { depth }
			, _stencil { stencil }
		{}

		Math::Color _color = { 0.f, 0.f, 0.f };
		float _depth = 1.0f;
		uint32 _stencil = 0;
	};

	struct CommandBufferFlags
	{
		enum 
		{
			Secondary = (1 << 0),
			MultiSubmit = (1 << 1),
			ImmediateSubmit = (1 << 2),
		};

	};

    struct CommandBufferDesc
    {
        CommandBufferDesc() = default;
        CommandBufferDesc(const CommandBufferDesc&) = default;
        CommandBufferDesc& operator = (const CommandBufferDesc&) = default;

        //! Constructs the command buffer descriptor with the specified flags.
        inline CommandBufferDesc(long flags) :
            flags{ flags }
        {
        }

        //! Constructs the command buffer descriptor with the specified flags and number of native buffers.
        inline CommandBufferDesc(long flags, std::uint32_t numNativeBuffers) :
            flags{ flags },
            numNativeBuffers{ numNativeBuffers }
        {
        }

        long            flags = 0;
        std::uint32_t   numNativeBuffers = 2;
    };

}