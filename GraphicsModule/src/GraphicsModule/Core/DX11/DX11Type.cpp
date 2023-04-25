#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Type.h"

namespace Graphics
{
	namespace DX11
	{
		DXGI_FORMAT MapFormat(const DataType dataType)
		{
			switch (dataType)
			{
				case DataType::Undefined:   break;//return DXGI_FORMAT_UNKNOWN;
				case DataType::Int8:        return DXGI_FORMAT_R8_SINT;
				case DataType::UInt8:       return DXGI_FORMAT_R8_UINT;
				case DataType::Int16:       return DXGI_FORMAT_R16_SINT;
				case DataType::UInt16:      return DXGI_FORMAT_R16_UINT;
				case DataType::Int32:       return DXGI_FORMAT_R32_SINT;
				case DataType::UInt32:      return DXGI_FORMAT_R32_UINT;
				case DataType::Float16:     return DXGI_FORMAT_R16_FLOAT;
				case DataType::Float32:     return DXGI_FORMAT_R32_FLOAT;
				case DataType::Float64:     break;
			}

			assert(false);
			return DXGI_FORMAT_UNKNOWN;
		}

		DXGI_FORMAT MapFormat(const Format format)
		{
			switch (format)
			{
				case Format::UNKNOWN:         return DXGI_FORMAT_UNKNOWN;

					/* --- Alpha channel color formats --- */
				case Format::A8_UNORM:           return DXGI_FORMAT_A8_UNORM;

					/* --- Red channel color formats --- */
				case Format::R8_UNORM:           return DXGI_FORMAT_R8_UNORM;
				case Format::R8_SNORM:           return DXGI_FORMAT_R8_SNORM;
				case Format::R8_UINT:            return DXGI_FORMAT_R8_UINT;
				case Format::R8_SINT:            return DXGI_FORMAT_R8_SINT;

				case Format::R16_UNORM:          return DXGI_FORMAT_R16_UNORM;
				case Format::R16_SNORM:          return DXGI_FORMAT_R16_SNORM;
				case Format::R16_UINT:           return DXGI_FORMAT_R16_UINT;
				case Format::R16_SINT:           return DXGI_FORMAT_R16_SINT;
				case Format::R16_FLOAT:          return DXGI_FORMAT_R16_FLOAT;

				case Format::R32_UINT:           return DXGI_FORMAT_R32_UINT;
				case Format::R32_SINT:           return DXGI_FORMAT_R32_SINT;
				case Format::R32_FLOAT:          return DXGI_FORMAT_R32_FLOAT;

					/* --- RG color formats --- */
				case Format::R8G8_UNORM:          return DXGI_FORMAT_R8G8_UNORM;
				case Format::R8G8_SNORM:          return DXGI_FORMAT_R8G8_SNORM;
				case Format::R8G8_UINT:           return DXGI_FORMAT_R8G8_UINT;
				case Format::R8G8_SINT:           return DXGI_FORMAT_R8G8_SINT;

				case Format::R16G16_UNORM:         return DXGI_FORMAT_R16G16_UNORM;
				case Format::R16G16_SNORM:         return DXGI_FORMAT_R16G16_SNORM;
				case Format::R16G16_UINT:          return DXGI_FORMAT_R16G16_UINT;
				case Format::R16G16_SINT:          return DXGI_FORMAT_R16G16_SINT;
				case Format::R16G16_FLOAT:         return DXGI_FORMAT_R16G16_FLOAT;

				case Format::R32G32_UINT:			return DXGI_FORMAT_R32G32_UINT;
				case Format::R32G32_SINT:			return DXGI_FORMAT_R32G32_SINT;
				case Format::R32G32_FLOAT:			return DXGI_FORMAT_R32G32_FLOAT;

				case Format::R32G32B32_UINT:		return DXGI_FORMAT_R32G32B32_UINT;
				case Format::R32G32B32_SINT:		return DXGI_FORMAT_R32G32B32_SINT;
				case Format::R32G32B32_FLOAT:		return DXGI_FORMAT_R32G32B32_FLOAT;

					/* --- RGBA color formats --- */
				case Format::R8G8B8A8_UNORM:		return DXGI_FORMAT_R8G8B8A8_UNORM;
				case Format::R8G8B8A8_UNORM_SRGB:	return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				case Format::R8G8B8A8_SNORM:		return DXGI_FORMAT_R8G8B8A8_SNORM;
				case Format::R8G8B8A8_UINT:			return DXGI_FORMAT_R8G8B8A8_UINT;
				case Format::R8G8B8A8_SINT:			return DXGI_FORMAT_R8G8B8A8_SINT;

				case Format::R16G16B16A16_UNORM:	return DXGI_FORMAT_R16G16B16A16_UNORM;
				case Format::R16G16B16A16_SNORM:	return DXGI_FORMAT_R16G16B16A16_SNORM;
				case Format::R16G16B16A16_UINT:		return DXGI_FORMAT_R16G16B16A16_UINT;
				case Format::R16G16B16A16_SINT:		return DXGI_FORMAT_R16G16B16A16_SINT;
				case Format::R16G16B16A16_FLOAT:	return DXGI_FORMAT_R16G16B16A16_FLOAT;

				case Format::R32G32B32A32_UINT:		return DXGI_FORMAT_R32G32B32A32_UINT;
				case Format::R32G32B32A32_SINT:		return DXGI_FORMAT_R32G32B32A32_SINT;
				case Format::R32G32B32A32_FLOAT:	return DXGI_FORMAT_R32G32B32A32_FLOAT;

					/* --- BGRA color formats --- */
				case Format::B8G8R8A8_UNORM:		return DXGI_FORMAT_B8G8R8A8_UNORM;
				case Format::B8G8R8A8_UNORM_SRGB:	return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

					/* --- Packed formats --- */
				case Format::R10G10B10A2_UNORM:		return DXGI_FORMAT_R10G10B10A2_UNORM;
				case Format::R10G10B10A2_UINT:		return DXGI_FORMAT_R10G10B10A2_UINT;
				case Format::R11G11B10_FLOAT:		return DXGI_FORMAT_R11G11B10_FLOAT;
				case Format::R9G9B9E5_SHAREDEXP:	return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;

					/* --- Depth-stencil formats --- */
				case Format::R16_TYPELESS:      return DXGI_FORMAT_R16_TYPELESS;
				case Format::R32_TYPELESS:      return DXGI_FORMAT_R32_TYPELESS;
				case Format::R24G8_TYPELESS:    return DXGI_FORMAT_R24G8_TYPELESS;
				case Format::R32G8X24_TYPELESS: return DXGI_FORMAT_R32G8X24_TYPELESS;

					/* --- Block compression (BC) formats --- */
				case Format::BC1_UNORM:          return DXGI_FORMAT_BC1_UNORM;
				case Format::BC1_UNORM_SRGB:     return DXGI_FORMAT_BC1_UNORM_SRGB;
				case Format::BC2_UNORM:          return DXGI_FORMAT_BC2_UNORM;
				case Format::BC2_UNORM_SRGB:     return DXGI_FORMAT_BC2_UNORM_SRGB;
				case Format::BC3_UNORM:          return DXGI_FORMAT_BC3_UNORM;
				case Format::BC3_UNORM_SRGB:     return DXGI_FORMAT_BC3_UNORM_SRGB;
				case Format::BC4_UNORM:          return DXGI_FORMAT_BC4_UNORM;
				case Format::BC4_SNORM:          return DXGI_FORMAT_BC4_SNORM;
				case Format::BC5_UNORM:          return DXGI_FORMAT_BC5_UNORM;
				case Format::BC5_SNORM:          return DXGI_FORMAT_BC5_SNORM;
			}
			assert(false);
			return DXGI_FORMAT_UNKNOWN;
		}

		D3D_PRIMITIVE_TOPOLOGY MapPrimitive(const PrimitiveTopology topology)
		{
			switch (topology)
			{
				case PrimitiveTopology::PointList:              return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
				case PrimitiveTopology::LineList:               return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
				case PrimitiveTopology::LineStrip:              return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
				case PrimitiveTopology::LineListAdjacency:      return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
				case PrimitiveTopology::LineStripAdjacency:     return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
				case PrimitiveTopology::TriangleList:           return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				case PrimitiveTopology::TriangleStrip:          return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
				case PrimitiveTopology::TriangleListAdjacency:  return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
				case PrimitiveTopology::TriangleStripAdjacency: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
				case PrimitiveTopology::Patches1:               return D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches2:               return D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches3:               return D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches4:               return D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches5:               return D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches6:               return D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches7:               return D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches8:               return D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches9:               return D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches10:              return D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches11:              return D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches12:              return D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches13:              return D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches14:              return D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches15:              return D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches16:              return D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches17:              return D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches18:              return D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches19:              return D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches20:              return D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches21:              return D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches22:              return D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches23:              return D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches24:              return D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches25:              return D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches26:              return D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches27:              return D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches28:              return D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches29:              return D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches30:              return D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches31:              return D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
				case PrimitiveTopology::Patches32:              return D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
			}

			assert(false);
			return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}

		D3D11_FILL_MODE MapFillMode(const FillMode polygonMode)
		{
			switch (polygonMode)
			{
				case FillMode::Solid:		return D3D11_FILL_SOLID;
				case FillMode::WireFrame:	return D3D11_FILL_WIREFRAME;
			}

			assert(false);
			return D3D11_FILL_SOLID;
		}

		D3D11_CULL_MODE MapCullMode(const CullMode cullMode)
		{
			switch (cullMode)
			{
				case CullMode::None:	return D3D11_CULL_NONE;
				case CullMode::Front:	return D3D11_CULL_FRONT;
				case CullMode::Back:	return D3D11_CULL_BACK;
			}

			assert(false);
			return D3D11_CULL_NONE;
		}

		D3D11_BLEND MapBlend(const Blend blend)
		{
			switch (blend)
			{
				case Blend::Zero:             return D3D11_BLEND_ZERO;
				case Blend::One:              return D3D11_BLEND_ONE;
				case Blend::SrcColor:         return D3D11_BLEND_SRC_COLOR;
				case Blend::InvSrcColor:      return D3D11_BLEND_INV_SRC_COLOR;
				case Blend::SrcAlpha:         return D3D11_BLEND_SRC_ALPHA;
				case Blend::InvSrcAlpha:      return D3D11_BLEND_INV_SRC_ALPHA;
				case Blend::DstColor:         return D3D11_BLEND_DEST_COLOR;
				case Blend::InvDstColor:      return D3D11_BLEND_INV_DEST_COLOR;
				case Blend::DstAlpha:         return D3D11_BLEND_DEST_ALPHA;
				case Blend::InvDstAlpha:      return D3D11_BLEND_INV_DEST_ALPHA;
				case Blend::SrcAlphaSaturate: return D3D11_BLEND_SRC_ALPHA_SAT;
				case Blend::BlendFactor:      return D3D11_BLEND_BLEND_FACTOR;
				case Blend::InvBlendFactor:   return D3D11_BLEND_INV_BLEND_FACTOR;
				case Blend::Src1Color:        return D3D11_BLEND_SRC1_COLOR;
				case Blend::InvSrc1Color:     return D3D11_BLEND_INV_SRC1_COLOR;
				case Blend::Src1Alpha:        return D3D11_BLEND_SRC1_ALPHA;
				case Blend::InvSrc1Alpha:     return D3D11_BLEND_INV_SRC1_ALPHA;
			}

			assert(false);
			return D3D11_BLEND_ZERO;
		}

		D3D11_BLEND_OP MapBlendOp(const BlendOp blendOp)
		{
			switch (blendOp)
			{
				case BlendOp::Add:          return D3D11_BLEND_OP_ADD;
				case BlendOp::Subtract:     return D3D11_BLEND_OP_SUBTRACT;
				case BlendOp::RevSubtract:  return D3D11_BLEND_OP_REV_SUBTRACT;
				case BlendOp::Min:          return D3D11_BLEND_OP_MIN;
				case BlendOp::Max:          return D3D11_BLEND_OP_MAX;
			}

			assert(false);
			return D3D11_BLEND_OP_ADD;
		}

		D3D11_COMPARISON_FUNC MapCompareOp(const CompareOp compareOp)
		{
			switch (compareOp)
			{
				case CompareOp::NeverPass:      return D3D11_COMPARISON_NEVER;
				case CompareOp::Less:           return D3D11_COMPARISON_LESS;
				case CompareOp::Equal:          return D3D11_COMPARISON_EQUAL;
				case CompareOp::LessEqual:      return D3D11_COMPARISON_LESS_EQUAL;
				case CompareOp::Greater:        return D3D11_COMPARISON_GREATER;
				case CompareOp::NotEqual:       return D3D11_COMPARISON_NOT_EQUAL;
				case CompareOp::GreaterEqual:   return D3D11_COMPARISON_GREATER_EQUAL;
				case CompareOp::AlwaysPass:     return D3D11_COMPARISON_ALWAYS;
			}

			assert(false);
			return D3D11_COMPARISON_NEVER;
		}

		D3D11_STENCIL_OP MapStencilOp(const StencilOp stencilOp)
		{
			switch (stencilOp)
			{
				case StencilOp::Keep:       return D3D11_STENCIL_OP_KEEP;
				case StencilOp::Zero:       return D3D11_STENCIL_OP_ZERO;
				case StencilOp::Replace:    return D3D11_STENCIL_OP_REPLACE;
				case StencilOp::IncClamp:   return D3D11_STENCIL_OP_INCR_SAT;
				case StencilOp::DecClamp:   return D3D11_STENCIL_OP_DECR_SAT;
				case StencilOp::Invert:     return D3D11_STENCIL_OP_INVERT;
				case StencilOp::IncWrap:    return D3D11_STENCIL_OP_INCR;
				case StencilOp::DecWrap:    return D3D11_STENCIL_OP_DECR;
			}

			assert(false);
			return D3D11_STENCIL_OP_KEEP;
		}

		D3D11_FILTER MapFilter(const Filter filter)
		{
			switch (filter)
			{
				case Filter::MIN_MAG_MIP_POINT:
					return D3D11_FILTER_MIN_MAG_MIP_POINT;

				case Filter::MIN_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;

				case Filter::MIN_POINT_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;

				case Filter::MIN_POINT_MAG_MIP_LINEAR:
					return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;

				case Filter::MIN_LINEAR_MAG_MIP_POINT:
					return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

				case Filter::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

				case Filter::MIN_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;

				case Filter::MIN_MAG_MIP_LINEAR:
					return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

				case Filter::ANISOTROPIC:
					return D3D11_FILTER_ANISOTROPIC;

				case Filter::COMPARISON_MIN_MAG_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

				case Filter::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;

				case Filter::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;

				case Filter::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;

				case Filter::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;

				case Filter::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

				case Filter::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;

				case Filter::COMPARISON_MIN_MAG_MIP_LINEAR:
					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

				case Filter::COMPARISON_ANISOTROPIC:
					return D3D11_FILTER_COMPARISON_ANISOTROPIC;

				case Filter::MINIMUM_MIN_MAG_MIP_POINT:
					return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;

				case Filter::MINIMUM_MIN_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;

				case Filter::MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;

				case Filter::MINIMUM_MIN_POINT_MAG_MIP_LINEAR:
					return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;

				case Filter::MINIMUM_MIN_LINEAR_MAG_MIP_POINT:
					return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;

				case Filter::MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

				case Filter::MINIMUM_MIN_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;

				case Filter::MINIMUM_MIN_MAG_MIP_LINEAR:
					return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;

				case Filter::MINIMUM_ANISOTROPIC:
					return D3D11_FILTER_MINIMUM_ANISOTROPIC;

				case Filter::MAXIMUM_MIN_MAG_MIP_POINT:
					return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;

				case Filter::MAXIMUM_MIN_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;

				case Filter::MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;

				case Filter::MAXIMUM_MIN_POINT_MAG_MIP_LINEAR:
					return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;

				case Filter::MAXIMUM_MIN_LINEAR_MAG_MIP_POINT:
					return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;

				case Filter::MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
					return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;

				case Filter::MAXIMUM_MIN_MAG_LINEAR_MIP_POINT:
					return D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;

				case Filter::MAXIMUM_MIN_MAG_MIP_LINEAR:
					return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;

				case Filter::MAXIMUM_ANISOTROPIC:
					return D3D11_FILTER_MAXIMUM_ANISOTROPIC;
			}

			assert(false);
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}

		D3D11_TEXTURE_ADDRESS_MODE MapSamplerMode(const SamplerAddressMode addressMode)
		{
			switch (addressMode)
			{
				case SamplerAddressMode::Wrap:			return D3D11_TEXTURE_ADDRESS_WRAP;
				case SamplerAddressMode::Mirror:		return D3D11_TEXTURE_ADDRESS_MIRROR;
				case SamplerAddressMode::Clamp:			return D3D11_TEXTURE_ADDRESS_CLAMP;
				case SamplerAddressMode::Border:		return D3D11_TEXTURE_ADDRESS_BORDER;
				case SamplerAddressMode::MirrorOnce:	return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
			}

			assert(false);
			return D3D11_TEXTURE_ADDRESS_WRAP;
		}

		D3D11_MAP MapCpuFlag(const CPUAccess cpuAccess)
		{
			switch (cpuAccess)
			{
				case CPUAccess::ReadOnly:		return D3D11_MAP_READ;
				case CPUAccess::ReadWrite:		return D3D11_MAP_READ_WRITE;
				case CPUAccess::WriteDiscard:	return D3D11_MAP_WRITE_DISCARD;
				case CPUAccess::WriteOnly:		return D3D11_MAP_WRITE;
			}

			assert(false);
			return D3D11_MAP_READ;
		}

		uint32 MapBufferBindFlag(const uint32 bindFlags)
		{
			uint32 _ret = 0;

			if ((bindFlags & BindFlags::VertexBuffer) != 0)
			{
				_ret |= D3D11_BIND_VERTEX_BUFFER;
			}

			if ((bindFlags & BindFlags::IndexBuffer) != 0)
			{
				_ret |= D3D11_BIND_INDEX_BUFFER;
			}

			if ((bindFlags & BindFlags::ConstantBuffer) != 0)
			{
				_ret |= D3D11_BIND_CONSTANT_BUFFER;
			}

			if ((bindFlags & BindFlags::StreamOutput) != 0)
			{
				_ret |= D3D11_BIND_STREAM_OUTPUT;
			}

			return _ret;
		}

		uint32 MapTextureBindFlag(const uint32 bindFlags)
		{
			uint32 _ret = 0;

			if ((bindFlags & BindFlags::ShaderResource) != 0)
			{
				_ret |= D3D11_BIND_SHADER_RESOURCE;
			}

			if ((bindFlags & BindFlags::RenderTarget) != 0)
			{
				_ret |= D3D11_BIND_RENDER_TARGET;
			}

			if ((bindFlags & BindFlags::DepthStencil) != 0)
			{
				_ret |= D3D11_BIND_DEPTH_STENCIL;
			}

			if ((bindFlags & BindFlags::UnorderedAccess) != 0)
			{
				_ret |= D3D11_BIND_UNORDERED_ACCESS;
			}

			return _ret;
		}

		uint32 MapCPUAcessFlagsForMiscFlags(const uint32 miscFlags)
		{
			uint32 flagsD3D = 0;

			if ((miscFlags & MiscFlags::DynamicUsage) != 0)
				flagsD3D |= D3D11_CPU_ACCESS_WRITE;

			return flagsD3D;
		}

		Format UnmapFormat(const DXGI_FORMAT format)
		{
			switch (format)
			{
				case DXGI_FORMAT_UNKNOWN:				return Format::UNKNOWN;
				case DXGI_FORMAT_A8_UNORM:				return Format::A8_UNORM;
				case DXGI_FORMAT_R8_UNORM:				return Format::R8_UNORM;
				case DXGI_FORMAT_R8_SNORM:				return Format::R8_SNORM;
				case DXGI_FORMAT_R8_UINT:				return Format::R8_UINT;
				case DXGI_FORMAT_R8_SINT:				return Format::R8_SINT;

				case DXGI_FORMAT_R16_UNORM:				return Format::R16_UNORM;
				case DXGI_FORMAT_R16_SNORM:				return Format::R16_SNORM;
				case DXGI_FORMAT_R16_UINT:				return Format::R16_UINT;
				case DXGI_FORMAT_R16_SINT:				return Format::R16_SINT;
				case DXGI_FORMAT_R16_FLOAT:				return Format::R16_FLOAT;

				case DXGI_FORMAT_R32_UINT:				return Format::R32_UINT;
				case DXGI_FORMAT_R32_SINT:				return Format::R32_SINT;
				case DXGI_FORMAT_R32_FLOAT:				return Format::R32_FLOAT;

				case DXGI_FORMAT_R8G8_UNORM:			return Format::R8G8_UNORM;
				case DXGI_FORMAT_R8G8_SNORM:			return Format::R8G8_SNORM;
				case DXGI_FORMAT_R8G8_UINT:				return Format::R8G8_UINT;
				case DXGI_FORMAT_R8G8_SINT:				return Format::R8G8_SINT;

				case DXGI_FORMAT_R16G16_UNORM:			return Format::R16G16_UNORM;
				case DXGI_FORMAT_R16G16_SNORM:			return Format::R16G16_SNORM;
				case DXGI_FORMAT_R16G16_UINT:			return Format::R16G16_UINT;
				case DXGI_FORMAT_R16G16_SINT:			return Format::R16G16_SINT;
				case DXGI_FORMAT_R16G16_FLOAT:			return Format::R16G16_FLOAT;

				case DXGI_FORMAT_R32G32_UINT:			return Format::R32G32_UINT;
				case DXGI_FORMAT_R32G32_SINT:			return Format::R32G32_SINT;
				case DXGI_FORMAT_R32G32_FLOAT:			return Format::R32G32_FLOAT;

				case DXGI_FORMAT_R32G32B32_UINT:		return Format::R32G32B32_UINT;
				case DXGI_FORMAT_R32G32B32_SINT:		return Format::R32G32B32_SINT;
				case DXGI_FORMAT_R32G32B32_FLOAT:		return Format::R32G32B32_FLOAT;

					/* --- RGBA color formats --- */
				case DXGI_FORMAT_R8G8B8A8_UNORM:		return Format::R8G8B8A8_UNORM;
				case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:	return Format::R8G8B8A8_UNORM_SRGB;
				case DXGI_FORMAT_R8G8B8A8_SNORM:		return Format::R8G8B8A8_SNORM;
				case DXGI_FORMAT_R8G8B8A8_UINT:			return Format::R8G8B8A8_UINT;
				case DXGI_FORMAT_R8G8B8A8_SINT:			return Format::R8G8B8A8_SINT;

				case DXGI_FORMAT_R16G16B16A16_UNORM:	return Format::R16G16B16A16_UNORM;
				case DXGI_FORMAT_R16G16B16A16_SNORM:	return Format::R16G16B16A16_SNORM;
				case DXGI_FORMAT_R16G16B16A16_UINT:		return Format::R16G16B16A16_UINT;
				case DXGI_FORMAT_R16G16B16A16_SINT:		return Format::R16G16B16A16_SINT;
				case DXGI_FORMAT_R16G16B16A16_FLOAT:	return Format::R16G16B16A16_FLOAT;

				case DXGI_FORMAT_R32G32B32A32_UINT:		return Format::R32G32B32A32_UINT;
				case DXGI_FORMAT_R32G32B32A32_SINT:		return Format::R32G32B32A32_SINT;
				case DXGI_FORMAT_R32G32B32A32_FLOAT:	return Format::R32G32B32A32_FLOAT;

					/* --- BGRA color formats --- */
				case DXGI_FORMAT_B8G8R8A8_UNORM:		return Format::B8G8R8A8_UNORM;
				case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:	return Format::B8G8R8A8_UNORM_SRGB;

					/* --- Packed formats --- */
				case DXGI_FORMAT_R10G10B10A2_UNORM:		return Format::R10G10B10A2_UNORM;
				case DXGI_FORMAT_R10G10B10A2_UINT:		return Format::R10G10B10A2_UINT;
				case DXGI_FORMAT_R11G11B10_FLOAT:		return Format::R11G11B10_FLOAT;
				case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:	return Format::R9G9B9E5_SHAREDEXP;

					/* --- Depth-stencil formats --- */
				case DXGI_FORMAT_R16_TYPELESS:			return Format::R16_TYPELESS;
				case DXGI_FORMAT_R32_TYPELESS:			return Format::R32_TYPELESS;
				case DXGI_FORMAT_R24G8_TYPELESS:		return Format::R24G8_TYPELESS;
				case DXGI_FORMAT_R32G8X24_TYPELESS:		return Format::R32G8X24_TYPELESS;

					/* --- Block compression (BC) formats --- */
				case DXGI_FORMAT_BC1_UNORM:				return Format::BC1_UNORM;
				case DXGI_FORMAT_BC1_UNORM_SRGB:		return Format::BC1_UNORM_SRGB;
				case DXGI_FORMAT_BC2_UNORM:				return Format::BC2_UNORM;
				case DXGI_FORMAT_BC2_UNORM_SRGB:		return Format::BC2_UNORM_SRGB;
				case DXGI_FORMAT_BC3_UNORM:				return Format::BC3_UNORM;
				case DXGI_FORMAT_BC3_UNORM_SRGB:		return Format::BC3_UNORM_SRGB;
				case DXGI_FORMAT_BC4_UNORM:				return Format::BC4_UNORM;
				case DXGI_FORMAT_BC4_SNORM:				return Format::BC4_SNORM;
				case DXGI_FORMAT_BC5_UNORM:				return Format::BC5_UNORM;
				case DXGI_FORMAT_BC5_SNORM:				return Format::BC5_SNORM;
			}

			return Format::UNKNOWN;
		}

		DXGI_FORMAT ToDXGIFormatDSV(const DXGI_FORMAT format)
		{
			switch (format)
			{
				case DXGI_FORMAT_R16_TYPELESS:      return DXGI_FORMAT_D16_UNORM;
				case DXGI_FORMAT_R32_TYPELESS:      return DXGI_FORMAT_D32_FLOAT;
				case DXGI_FORMAT_R24G8_TYPELESS:    return DXGI_FORMAT_D24_UNORM_S8_UINT;
				case DXGI_FORMAT_R32G8X24_TYPELESS: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
				default:                            return format;
			}
		}

		DXGI_FORMAT ToDXGIFormatSRV(const DXGI_FORMAT format)
		{
			switch (format)
			{
				case DXGI_FORMAT_R16_TYPELESS:      return DXGI_FORMAT_R16_UNORM;
				case DXGI_FORMAT_R32_TYPELESS:      return DXGI_FORMAT_R32_FLOAT;
				case DXGI_FORMAT_R24G8_TYPELESS:    return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				case DXGI_FORMAT_R32G8X24_TYPELESS: return DXGI_FORMAT_UNKNOWN;
				default:                            return format;
			}
		}

		DXGI_FORMAT ToDXGIFormatUAV(const DXGI_FORMAT format)
		{
			switch (format)
			{
				case DXGI_FORMAT_R16_TYPELESS:          return DXGI_FORMAT_R16_UNORM;
				case DXGI_FORMAT_R32_TYPELESS:          return DXGI_FORMAT_R32_FLOAT;
				case DXGI_FORMAT_R24G8_TYPELESS:        return DXGI_FORMAT_UNKNOWN;
				case DXGI_FORMAT_R32G8X24_TYPELESS:     return DXGI_FORMAT_UNKNOWN;
				case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM; // UAVs cannot have typed sRGB format
				case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8A8_UNORM; // UAVs cannot have typed sRGB format
				default:                                return format;
			}
		}
	}
}