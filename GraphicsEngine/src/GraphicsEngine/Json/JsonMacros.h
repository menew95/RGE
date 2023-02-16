#pragma once

#include "JsonReader/JsonMacro.h"

/**** RenderTarget Enum ****/

JSON_SERIALIZE_ENUM(Graphics::RenderTargetType,
	{
	{Graphics::RenderTargetType::Depth, _T("Depth")},
	{Graphics::RenderTargetType::DepthStencil, _T("DepthStencil")},
	{Graphics::RenderTargetType::RenderTarget, _T("RenderTarget")},
	{Graphics::RenderTargetType::Stencil, _T("Stencil")}
	});

/**** Texture Enum ****/

JSON_SERIALIZE_ENUM(Graphics::TextureType,
	{
	{Graphics::TextureType::Texture1D, _T("Texture1D")},
	{Graphics::TextureType::Texture2D, _T("Texture2D")},
	{Graphics::TextureType::Texture3D, _T("Texture3D")},
	{Graphics::TextureType::TextureCube, _T("TextureCube")},
	{Graphics::TextureType::Texture1DArray, _T("Texture1DArray")},
	{Graphics::TextureType::Texture2DArray, _T("Texture2DArray")},
	{Graphics::TextureType::TextureCubeArray, _T("TextureCubeArray")},
	{Graphics::TextureType::Texture2DMS, _T("Texture2DMS")},
	{Graphics::TextureType::Texture2DMSArray, _T("Texture2DMSArray")}
	});

JSON_SERIALIZE_ENUM(Graphics::Format,
	{
	{Graphics::Format::R8G8B8A8_UNORM, _T("R8G8B8A8_UNORM")},
	{Graphics::Format::R32G32B32A32_FLOAT, _T("R32G32B32A32_FLOAT")},
	{Graphics::Format::R24G8_TYPELESS, _T("R24G8_TYPELESS")},
	{Graphics::Format::R32_TYPELESS, _T("R32_TYPELESS")}
	});


JSON_SERIALIZE_ENUM(Graphics::MiscFlags::eMiscFlags,
	{
	{Graphics::MiscFlags::DynamicUsage, _T("DynamicUsage")},
	{Graphics::MiscFlags::FixedSamples, _T("FixedSamples")},
	{Graphics::MiscFlags::GenerateMips, _T("GenerateMips")},
	{Graphics::MiscFlags::NoInitialData, _T("NoInitialData")},
	{Graphics::MiscFlags::Append, _T("Append")},
	{Graphics::MiscFlags::Counter, _T("Counter")}
	});

JSON_SERIALIZE_ENUM(Graphics::BindFlags::eBindFlags,
	{
	{Graphics::BindFlags::VertexBuffer, _T("VertexBuffer")},
	{Graphics::BindFlags::IndexBuffer, _T("IndexBuffer")},
	{Graphics::BindFlags::ConstantBuffer, _T("ConstantBuffer")},
	{Graphics::BindFlags::ShaderResource, _T("ShaderResource")},
	{Graphics::BindFlags::StreamOutput, _T("StreamOutput")},
	{Graphics::BindFlags::RenderTarget, _T("RenderTarget")},
	{Graphics::BindFlags::DepthStencil, _T("DepthStencil")},
	{Graphics::BindFlags::UnorderedAccess, _T("UnorderedAccess")},
	{Graphics::BindFlags::Decoder, _T("Decoder")},
	{Graphics::BindFlags::VideoEncoder, _T("VideoEncoder")}
	});

JSON_SERIALIZE_ENUM(Graphics::StageFlags::eStageFlags,
	{
	{Graphics::StageFlags::VS, _T("VS")},
	{Graphics::StageFlags::HS, _T("HS")},
	{Graphics::StageFlags::DS, _T("DS")},
	{Graphics::StageFlags::GS, _T("GS")},
	{Graphics::StageFlags::PS, _T("PS")},
	{Graphics::StageFlags::CS, _T("CS")},
	{Graphics::StageFlags::AllTessStages, _T("AllTessStages")},
	{Graphics::StageFlags::AllGraphicsStages, _T("AllGraphicsStages")},
	{Graphics::StageFlags::AllStages, _T("AllStages")}
	});

JSON_SERIALIZE_ENUM(Graphics::ResourceType,
	{
	{Graphics::ResourceType::Undefined, _T("Undefined")},
	{Graphics::ResourceType::Buffer, _T("Buffer")},
	{Graphics::ResourceType::Texture, _T("Texture")},
	{Graphics::ResourceType::Sampler, _T("Sampler")}
	});

/**** Shader Enum ****/

JSON_SERIALIZE_ENUM(Graphics::ShaderType,
	{
	{Graphics::ShaderType::Vertex, _T("Vertex")},
	{Graphics::ShaderType::Pixel, _T("Pixel")},
	{Graphics::ShaderType::Geometry, _T("Geometry")},
	{Graphics::ShaderType::Hull, _T("Hull")},
	{Graphics::ShaderType::Domain, _T("Domain")},
	{Graphics::ShaderType::Compute, _T("Compute")},
	{Graphics::ShaderType::Reserved0, _T("Reserved0")}
	});

JSON_SERIALIZE_ENUM(Graphics::ShaderSourceType,
	{
	{Graphics::ShaderSourceType::CSO, _T("CSO")},
	{Graphics::ShaderSourceType::HLSL, _T("HLSL")}
	});

/**** Pipeline-State enum ****/

JSON_SERIALIZE_ENUM(Graphics::CompareOp,
	{
	{Graphics::CompareOp::NeverPass,    _T("NeverPass")},
	{Graphics::CompareOp::Less,         _T("Less")},
	{Graphics::CompareOp::Equal,        _T("Equal")},
	{Graphics::CompareOp::LessEqual,    _T("LessEqual")},
	{Graphics::CompareOp::Greater,      _T("Greater")},
	{Graphics::CompareOp::NotEqual,     _T("NotEqual")},
	{Graphics::CompareOp::GreaterEqual, _T("GreaterEqual")},
	{Graphics::CompareOp::AlwaysPass,   _T("AlwaysPass")}
	});

JSON_SERIALIZE_ENUM(Graphics::StencilOp,
	{
	{Graphics::StencilOp::Keep    , _T("Keep")},
	{Graphics::StencilOp::Zero    , _T("Zero")},
	{Graphics::StencilOp::Replace , _T("Replace")},
	{Graphics::StencilOp::IncClamp, _T("IncClamp")},
	{Graphics::StencilOp::DecClamp, _T("DecClamp")},
	{Graphics::StencilOp::Invert  , _T("Invert")},
	{Graphics::StencilOp::IncWrap , _T("IncWrap")},
	{Graphics::StencilOp::DecWrap , _T("DecWrap")}
	});

JSON_SERIALIZE_ENUM(Graphics::FillMode,
	{
	{Graphics::FillMode::WireFrame, _T("WireFrame")},
	{Graphics::FillMode::Solid, _T("Solid")}
	});

JSON_SERIALIZE_ENUM(Graphics::CullMode,
	{
	{Graphics::CullMode::None, _T("None")},
	{Graphics::CullMode::Front, _T("Front")},
	{Graphics::CullMode::Back, _T("Back")}
	});

JSON_SERIALIZE_ENUM(Graphics::Blend,
	{
	{Graphics::Blend::Zero,             _T("Zero")},
	{Graphics::Blend::One,              _T("One")},
	{Graphics::Blend::SrcColor,         _T("SrcColor")},
	{Graphics::Blend::InvSrcColor,      _T("InvSrcColor")},
	{Graphics::Blend::SrcAlpha,         _T("SrcAlpha")},
	{Graphics::Blend::InvSrcAlpha,      _T("InvSrcAlpha")},
	{Graphics::Blend::DstColor,         _T("DstColor")},
	{Graphics::Blend::InvDstColor,      _T("InvDstColor")},
	{Graphics::Blend::DstAlpha,         _T("DstAlpha")},
	{Graphics::Blend::InvDstAlpha,      _T("InvDstAlpha")},
	{Graphics::Blend::SrcAlphaSaturate, _T("SrcAlphaSaturate")},
	{Graphics::Blend::BlendFactor,      _T("BlendFactor")},
	{Graphics::Blend::InvBlendFactor,   _T("InvBlendFactor")},
	{Graphics::Blend::Src1Color,        _T("Src1Color")},
	{Graphics::Blend::InvSrc1Color,     _T("InvSrc1Color")},
	{Graphics::Blend::Src1Alpha,        _T("Src1Alpha")},
	{Graphics::Blend::InvSrc1Alpha,      _T("InvSrc1Alpha")}
	});

JSON_SERIALIZE_ENUM(Graphics::BlendOp,
	{
	{Graphics::BlendOp::Add, _T("Add")},
	{Graphics::BlendOp::Subtract, _T("Subtract")},
	{Graphics::BlendOp::RevSubtract, _T("RevSubtract")},
	{Graphics::BlendOp::Min, _T("Min")},
	{Graphics::BlendOp::Max, _T("Max")}
	});

JSON_SERIALIZE_ENUM(Graphics::PrimitiveTopology,
	{
	{Graphics::PrimitiveTopology::PointList, _T("PointList")},
	{Graphics::PrimitiveTopology::LineList, _T("LineList")},
	{Graphics::PrimitiveTopology::LineStrip, _T("LineStrip")},
	{Graphics::PrimitiveTopology::LineListAdjacency, _T("LineListAdjacency")},
	{Graphics::PrimitiveTopology::LineStripAdjacency, _T("LineStripAdjacency")},
	{Graphics::PrimitiveTopology::TriangleList, _T("TriangleList")},
	{Graphics::PrimitiveTopology::TriangleStrip, _T("TriangleStrip")},
	{Graphics::PrimitiveTopology::TriangleListAdjacency, _T("TriangleListAdjacency")},
	{Graphics::PrimitiveTopology::TriangleStripAdjacency, _T("TriangleStripAdjacency")},
	{Graphics::PrimitiveTopology::Patches1, _T("Patches1")},
	{Graphics::PrimitiveTopology::Patches2, _T("Patches2")},
	{Graphics::PrimitiveTopology::Patches3, _T("Patches3")},
	{Graphics::PrimitiveTopology::Patches4, _T("Patches4")},
	{Graphics::PrimitiveTopology::Patches5, _T("Patches5")},
	{Graphics::PrimitiveTopology::Patches6, _T("Patches6")},
	{Graphics::PrimitiveTopology::Patches7, _T("Patches7")},
	{Graphics::PrimitiveTopology::Patches8, _T("Patches8")},
	{Graphics::PrimitiveTopology::Patches9, _T("Patches9")},
	{Graphics::PrimitiveTopology::Patches10, _T("Patches10")},
	{Graphics::PrimitiveTopology::Patches11, _T("Patches11")},
	{Graphics::PrimitiveTopology::Patches12, _T("Patches12")},
	{Graphics::PrimitiveTopology::Patches13, _T("Patches13")},
	{Graphics::PrimitiveTopology::Patches14, _T("Patches14")},
	{Graphics::PrimitiveTopology::Patches15, _T("Patches15")},
	{Graphics::PrimitiveTopology::Patches16, _T("Patches16")},
	{Graphics::PrimitiveTopology::Patches17, _T("Patches17")},
	{Graphics::PrimitiveTopology::Patches18, _T("Patches18")},
	{Graphics::PrimitiveTopology::Patches19, _T("Patches19")},
	{Graphics::PrimitiveTopology::Patches20, _T("Patches20")},
	{Graphics::PrimitiveTopology::Patches21, _T("Patches21")},
	{Graphics::PrimitiveTopology::Patches22, _T("Patches22")},
	{Graphics::PrimitiveTopology::Patches23, _T("Patches23")},
	{Graphics::PrimitiveTopology::Patches24, _T("Patches24")},
	{Graphics::PrimitiveTopology::Patches25, _T("Patches25")},
	{Graphics::PrimitiveTopology::Patches26, _T("Patches26")},
	{Graphics::PrimitiveTopology::Patches27, _T("Patches27")},
	{Graphics::PrimitiveTopology::Patches28, _T("Patches28")},
	{Graphics::PrimitiveTopology::Patches29, _T("Patches29")},
	{Graphics::PrimitiveTopology::Patches30, _T("Patches30")},
	{Graphics::PrimitiveTopology::Patches31, _T("Patches31")},
	{Graphics::PrimitiveTopology::Patches32, _T("Patches32")},
	});

/**** Sampler Enum ****/

JSON_SERIALIZE_ENUM(Graphics::Filter,
	{
	{Graphics::Filter::MIN_MAG_MIP_POINT, _T("MIN_MAG_MIP_POINT")},
	{Graphics::Filter::MIN_MAG_POINT_MIP_LINEAR, _T("MIN_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::MIN_POINT_MAG_LINEAR_MIP_POINT, _T("MIN_POINT_MAG_LINEAR_MIP_POINT")},
	{Graphics::Filter::MIN_POINT_MAG_MIP_LINEAR, _T("MIN_POINT_MAG_MIP_LINEAR")},
	{Graphics::Filter::MIN_LINEAR_MAG_MIP_POINT, _T("MIN_LINEAR_MAG_MIP_POINT")},
	{Graphics::Filter::MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::MIN_MAG_LINEAR_MIP_POINT, _T("MIN_MAG_LINEAR_MIP_POINT")},
	{Graphics::Filter::MIN_MAG_MIP_LINEAR, _T("MIN_MAG_MIP_LINEAR")},
	{Graphics::Filter::ANISOTROPIC, _T("ANISOTROPIC")},
	{Graphics::Filter::COMPARISON_MIN_MAG_MIP_POINT, _T("COMPARISON_MIN_MAG_MIP_POINT")},
	{Graphics::Filter::COMPARISON_MIN_MAG_POINT_MIP_LINEAR, _T("COMPARISON_MIN_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT, _T("COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT")},
	{Graphics::Filter::COMPARISON_MIN_POINT_MAG_MIP_LINEAR, _T("COMPARISON_MIN_POINT_MAG_MIP_LINEAR")},
	{Graphics::Filter::COMPARISON_MIN_LINEAR_MAG_MIP_POINT, _T("COMPARISON_MIN_LINEAR_MAG_MIP_POINT")},
	{Graphics::Filter::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::COMPARISON_MIN_MAG_LINEAR_MIP_POINT, _T("COMPARISON_MIN_MAG_LINEAR_MIP_POINT")},
	{Graphics::Filter::COMPARISON_MIN_MAG_MIP_LINEAR, _T("COMPARISON_MIN_MAG_MIP_LINEAR")},
	{Graphics::Filter::COMPARISON_ANISOTROPIC, _T("COMPARISON_ANISOTROPIC")},
	{Graphics::Filter::MINIMUM_MIN_MAG_MIP_POINT, _T("MINIMUM_MIN_MAG_MIP_POINT")},
	{Graphics::Filter::MINIMUM_MIN_MAG_POINT_MIP_LINEAR, _T("MINIMUM_MIN_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, _T("MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT")},
	{Graphics::Filter::MINIMUM_MIN_POINT_MAG_MIP_LINEAR, _T("MINIMUM_MIN_POINT_MAG_MIP_LINEAR")},
	{Graphics::Filter::MINIMUM_MIN_LINEAR_MAG_MIP_POINT, _T("MINIMUM_MIN_LINEAR_MAG_MIP_POINT")},
	{Graphics::Filter::MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::MINIMUM_MIN_MAG_LINEAR_MIP_POINT, _T("MINIMUM_MIN_MAG_LINEAR_MIP_POINT")},
	{Graphics::Filter::MINIMUM_MIN_MAG_MIP_LINEAR, _T("MINIMUM_MIN_MAG_MIP_LINEAR")},
	{Graphics::Filter::MINIMUM_ANISOTROPIC, _T("MINIMUM_ANISOTROPIC")},
	{Graphics::Filter::MAXIMUM_MIN_MAG_MIP_POINT, _T("MAXIMUM_MIN_MAG_MIP_POINT")},
	{Graphics::Filter::MAXIMUM_MIN_MAG_POINT_MIP_LINEAR, _T("MAXIMUM_MIN_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, _T("MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POIN,")},
	{Graphics::Filter::MAXIMUM_MIN_POINT_MAG_MIP_LINEAR, _T("MAXIMUM_MIN_POINT_MAG_MIP_LINEAR")},
	{Graphics::Filter::MAXIMUM_MIN_LINEAR_MAG_MIP_POINT, _T("MAXIMUM_MIN_LINEAR_MAG_MIP_POINT")},
	{Graphics::Filter::MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, _T("MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR")},
	{Graphics::Filter::MAXIMUM_MIN_MAG_LINEAR_MIP_POINT, _T("MAXIMUM_MIN_MAG_LINEAR_MIP_POINT")},
	{Graphics::Filter::MAXIMUM_MIN_MAG_MIP_LINEAR, _T("MAXIMUM_MIN_MAG_MIP_LINEAR")},
	{Graphics::Filter::MAXIMUM_ANISOTROPIC, _T("MAXIMUM_ANISOTROPIC")}
	});

JSON_SERIALIZE_ENUM(Graphics::SamplerAddressMode,
	{
	{Graphics::SamplerAddressMode::Wrap, _T("Wrap")},
	{Graphics::SamplerAddressMode::Mirror, _T("Mirror")},
	{Graphics::SamplerAddressMode::Clamp, _T("Clamp")},
	{Graphics::SamplerAddressMode::Border, _T("Border")},
	{Graphics::SamplerAddressMode::MirrorOnce, _T("MirrorOnce")},
	});