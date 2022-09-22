#pragma once

#include "../ID3D.h"

namespace RenderCore {
	class DeviceContext;

	namespace AddressMode
	{
		enum Enum
		{
			Wrap = 1,   // D3D11_TEXTURE_ADDRESS_WRAP
			Mirror = 2, // D3D11_TEXTURE_ADDRESS_MIRROR
			Clamp = 3,  // D3D11_TEXTURE_ADDRESS_CLAMP
			Border = 4  // D3D11_TEXTURE_ADDRESS_BORDER
		};
	}
	namespace FilterMode
	{
		enum Enum
		{
			Point = 0,                  // D3D11_FILTER_MIN_MAG_MIP_POINT
			Trilinear = 0x15,           // D3D11_FILTER_MIN_MAG_MIP_LINEAR
			Anisotropic = 0x55,         // D3D11_FILTER_ANISOTROPIC
			Bilinear = 0x14,            // D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT
			ComparisonBilinear = 0x94   // D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT
		};
	}

	namespace Comparison
	{
		enum Enum
		{
			Never = 1,          // D3D11_COMPARISON_NEVER
			Less = 2,           // D3D11_COMPARISON_LESS
			Equal = 3,          // D3D11_COMPARISON_EQUAL
			LessEqual = 4,      // D3D11_COMPARISON_LESS_EQUAL
			Greater = 5,        // D3D11_COMPARISON_GREATER
			NotEqual = 6,       // D3D11_COMPARISON_NOT_EQUAL
			GreaterEqual = 7,   // D3D11_COMPARISON_GREATER_EQUAL
			Always = 8          // D3D11_COMPARISON_ALWAYS
		};
	}

	class SamplerState
	{
	public:
		SamplerState(FilterMode::Enum filter = FilterMode::Trilinear,
			AddressMode::Enum addressU = AddressMode::Wrap,
			AddressMode::Enum addressV = AddressMode::Wrap,
			AddressMode::Enum addressW = AddressMode::Wrap,
			Comparison::Enum comparison = Comparison::Never);
		~SamplerState();

		SamplerState(SamplerState&& moveFrom);
		SamplerState& operator=(SamplerState&& moveFrom);
		SamplerState(const SamplerState& copyFrom);
		SamplerState& operator=(const SamplerState& copyFrom);

		ID3D::SamplerState* GetState() const { return mState.Get(); }
		ID3D::SamplerState** GetStateRef() { return mState.GetAddressOf(); }
		
	private:
		ComPtr<ID3D::SamplerState>  mState;
	};

	namespace CullMode
	{
		enum Enum
		{
			None = 1,   // D3D11_CULL_NONE,
			Front = 2,  // D3D11_CULL_FRONT,
			Back = 3    // D3D11_CULL_BACK
		};
	};

	namespace FillMode
	{
		enum Enum
		{
			Solid = 3,      // D3D11_FILL_SOLID
			Wireframe = 2   // D3D11_FILL_WIREFRAME
		};
	};

	class RasterizerState
	{
	public:
		RasterizerState(CullMode::Enum cullmode = CullMode::Back, bool frontCounterClockwise = true);
		RasterizerState(
			CullMode::Enum cullmode, bool frontCounterClockwise,
			FillMode::Enum fillmode,
			int depthBias, float depthBiasClamp, float slopeScaledBias);
		RasterizerState(DeviceContext&);
		~RasterizerState();

		RasterizerState(RasterizerState&& moveFrom);
		RasterizerState& operator=(RasterizerState&& moveFrom);
		RasterizerState(const RasterizerState& copyFrom);
		RasterizerState& operator=(const RasterizerState& copyFrom);
		RasterizerState(ComPtr<ID3D::RasterizerState>&& moveFrom);

		ID3D::RasterizerState* GetState() const { mState.Get(); }
		ID3D::RasterizerState** GetStateRef() { mState.GetAddressOf(); }

		static RasterizerState Null();

	private:
		ComPtr<ID3D::RasterizerState>  mState;
	};
	namespace Blend
	{
		enum Enum
		{
			Zero = 1, // D3D11_BLEND_ZERO,
			One = 2, // D3D11_BLEND_ONE,

			SrcColor = 3, // D3D11_BLEND_SRC_COLOR,
			InvSrcColor = 4, // D3D11_BLEND_INV_SRC_COLOR,
			DestColor = 9, // D3D11_BLEND_DEST_COLOR,
			InvDestColor = 10, // D3D11_BLEND_INV_DEST_COLOR,

			SrcAlpha = 5, // D3D11_BLEND_SRC_ALPHA,
			InvSrcAlpha = 6, // D3D11_BLEND_INV_SRC_ALPHA,
			DestAlpha = 7, // D3D11_BLEND_DEST_ALPHA,
			InvDestAlpha = 8 // D3D11_BLEND_INV_DEST_ALPHA
		};
	};

	namespace BlendOp
	{
		enum Enum
		{
			NoBlending,
			Add = 1, // D3D11_BLEND_OP_ADD,
			Subtract = 2, // D3D11_BLEND_OP_SUBTRACT,
			RevSubtract = 3, // D3D11_BLEND_OP_REV_SUBTRACT,
			Min = 4, // D3D11_BLEND_OP_MIN,
			Max = 5 // D3D11_BLEND_OP_MAX
		};
	}

	class BlendState
	{
	public:
		BlendState(BlendOp::Enum blendingOperation = BlendOp::Add,
			Blend::Enum srcBlend = Blend::SrcAlpha,
			Blend::Enum dstBlend = Blend::InvSrcAlpha);
		BlendState(BlendOp::Enum blendingOperation,
			Blend::Enum srcBlend,
			Blend::Enum dstBlend,
			BlendOp::Enum alphaBlendingOperation,
			Blend::Enum alphaSrcBlend,
			Blend::Enum alphaDstBlend);
		~BlendState();

		BlendState(BlendState&& moveFrom);
		BlendState& operator=(BlendState&& moveFrom);
		BlendState(const BlendState& copyFrom);
		BlendState& operator=(const BlendState& copyFrom);
		BlendState(ComPtr<ID3D::BlendState>&& moveFrom);
		BlendState(DeviceContext& context);

		static BlendState OutputDisabled();
		static BlendState Null();

		ID3D::BlendState* GetState() const { return mState.Get(); }
		ID3D::BlendState** GetState() { return mState.GetAddressOf(); }

	private:
		ComPtr<ID3D::BlendState>  mState;
	};

	namespace StencilOp
	{
		enum Enum
		{
			DontWrite = 1,      // D3D11_STENCIL_OP_KEEP
			Zero = 2,           // D3D11_STENCIL_OP_ZERO
			Replace = 3,        // D3D11_STENCIL_OP_REPLACE
			IncreaseSat = 4,    // D3D11_STENCIL_OP_INCR_SAT
			DecreaseSat = 5,    // D3D11_STENCIL_OP_DECR_SAT
			Invert = 6,         // D3D11_STENCIL_OP_INVERT
			Increase = 7,       // D3D11_STENCIL_OP_INCR
			Decrease = 8        // D3D11_STENCIL_OP_DECR
		};
	}

	class StencilMode
	{
	public:
		Comparison::Enum _comparison;
		StencilOp::Enum _onPass;
		StencilOp::Enum _onDepthFail;
		StencilOp::Enum _onStencilFail;
		StencilMode(
			Comparison::Enum comparison = Comparison::Always,
			StencilOp::Enum onPass = StencilOp::Replace,
			StencilOp::Enum onStencilFail = StencilOp::DontWrite,
			StencilOp::Enum onDepthFail = StencilOp::DontWrite)
			: _comparison(comparison)
			, _onPass(onPass)
			, _onStencilFail(onStencilFail)
			, _onDepthFail(onDepthFail) {}

		static StencilMode NoEffect;
		static StencilMode AlwaysWrite;
	};

	class DepthStencilState
	{
	public:
		explicit DepthStencilState(bool enabled = true, bool writeEnabled = true, Comparison::Enum comparison = Comparison::LessEqual);
		DepthStencilState(
			bool depthTestEnabled, bool writeEnabled,
			unsigned stencilReadMask, unsigned stencilWriteMask,
			const StencilMode& frontFaceStencil = StencilMode::NoEffect,
			const StencilMode& backFaceStencil = StencilMode::NoEffect);
		DepthStencilState(DeviceContext& context);
		DepthStencilState(DepthStencilState&& moveFrom);
		DepthStencilState& operator=(DepthStencilState&& moveFrom);
		~DepthStencilState();

		ID3D::DepthStencilState* GetState() const { return mState.Get(); }
		ID3D::DepthStencilState** GetStateRef() { return mState.GetAddressOf(); }

	private:
		ComPtr<ID3D::DepthStencilState>  mState;
	};

	class DeviceContext;

	class ViewportDesc
	{
	public:
		// (compatible with D3D11_VIEWPORT)
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;

		ViewportDesc(ID3D::DeviceContext* context);
		ViewportDesc(const DeviceContext& context);
		ViewportDesc(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.f, float maxDepth = 1.f)
			: TopLeftX(topLeftX), TopLeftY(topLeftY), Width(width), Height(height)
			, MinDepth(minDepth), MaxDepth(maxDepth) {}
		ViewportDesc() : TopLeftX(0.f), TopLeftY(0.f), Width(0.f), Height(0.f)
			, MinDepth(0.f), MaxDepth(0.f) {}
	};
}
