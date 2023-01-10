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

		Math::Color _color = { 0.f, 0.f, 0.f, 0.f };
		float _depth = 1.0f;
		uint32 _stencil = 0;
	};

	struct ClearFlags
	{
		enum
		{
			Color = (1 << 0),
			Depth = (1 << 1),
			Stencil = (1 << 2),

			ColorDepth = (Color | Depth),
			DepthStencil = (Depth | Stencil),
			All = (Color | Depth | Stencil),
		};
	};

    struct AttachmentClear
    {
        AttachmentClear() = default;
        AttachmentClear(const AttachmentClear&) = default;
        AttachmentClear& operator = (const AttachmentClear&) = default;

        //! Constructor for a color attachment clear command.
        inline AttachmentClear(const Math::Color& color, uint32 colorAttachment) :
            _flags{ ClearFlags::Color },
            _colorAttachment{ colorAttachment }
        {
            _clearValue._color = color;
        }

        //! Constructor for a depth attachment clear command.
        inline AttachmentClear(float depth) :
            _flags{ ClearFlags::Depth }
        {
            _clearValue._depth = depth;
        }

        //! Constructor for a stencil attachment clear command.
        inline AttachmentClear(uint32 stencil) :
            _flags{ ClearFlags::Stencil }
        {
            _clearValue._stencil = stencil;
        }

        //! Constructor for a depth-stencil attachment clear command.
        inline AttachmentClear(float depth, uint32 stencil) :
            _flags{ ClearFlags::DepthStencil }
        {
			_clearValue._depth = depth;
			_clearValue._stencil = stencil;
        }

        long _flags = 0;

        uint32 _colorAttachment = 0;

        ClearValue _clearValue;
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
            _flags{ flags }
        {
        }

        //! Constructs the command buffer descriptor with the specified flags and number of native buffers.
        inline CommandBufferDesc(long flags, uint32 numNativeBuffers) :
            _flags{ flags },
            _numNativeBuffers{ numNativeBuffers }
        {
        }

        long _flags = 0;
        uint32 _numNativeBuffers = 2;
    };

}