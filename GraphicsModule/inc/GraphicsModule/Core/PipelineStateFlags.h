#pragma once

#include "Common.h"
#include "Math/Math.h"

namespace Graphics
{
	enum class CompareOp
	{
		NeverPass,    
		Less,         
		Equal,        
		LessEqual,    
		Greater,      
		NotEqual,     
		GreaterEqual, 
		AlwaysPass,   
	};

	struct DepthDesc
	{
		bool        _depthEnabled = false;
		bool        _writeEnabled = false;
		CompareOp   compareOp = CompareOp::Less;
	};

	enum class StencilOp
	{
		Keep,       
		Zero,       
		Replace,    
		IncClamp,   
		DecClamp,   
		Invert,     
		IncWrap,    
		DecWrap,    
	};

	struct StencilFaceDescriptor
	{
		StencilOp _stencilFailOp = StencilOp::Keep;
		StencilOp _depthFailOp = StencilOp::Keep;
		StencilOp _depthPassOp = StencilOp::Keep;
		CompareOp _compareOp = CompareOp::Less;
	};

	struct StencilDesc
	{
		bool _stencilEnable = false;
		uint8 _readMask;
		uint8 _writeMask;
		StencilFaceDescriptor _front;
		StencilFaceDescriptor _back;
	};

	enum class FillMode
	{
		WireFrame,
		Solid,
	};

	enum class CullMode
	{
		None,
		Front,
		Back,
	};

	//! Depth bias descriptor structure to control fragment depth values.
	struct DepthBiasDesc
	{
		float constantFactor = 0.0f;
		float slopeFactor = 0.0f;
		float clamp = 0.0f;
	};

	struct RasterizerDesc
	{
		FillMode _fillMode = FillMode::Solid;
		CullMode _cullMode = CullMode::None;
		DepthBiasDesc _depthBias;

		bool _frontCCW = false;
		bool _depthClampEnabled = false;
		bool _scissorTestEnabled = false;
		bool _multiSampleEnabled = false;
		bool _antiAliasedLineEnabled = false;
		bool _conservativeRasterization = false;
	};

	enum class Blend
	{
		Zero,              
		One,               
		SrcColor,          
		InvSrcColor,       
		SrcAlpha,          
		InvSrcAlpha,       
		DstColor,          
		InvDstColor,       
		DstAlpha,          
		InvDstAlpha,       
		SrcAlphaSaturate,  
		BlendFactor,       
		InvBlendFactor,    
		Src1Color,         
		InvSrc1Color,      
		Src1Alpha,         
		InvSrc1Alpha       
	};

	enum class BlendOp
	{
		Add,
		Subtract,
		RevSubtract,
		Min,
		Max,
	};

	struct BlendTargetDesc
	{
		bool _blendEnable = false;
		Blend _srcBlend = Blend::One;
		Blend _destBlend = Blend::Zero;
		BlendOp _blendOp = BlendOp::Add;
		Blend _srcBlendAlpha = Blend::One;
		Blend _destBlendAlpha = Blend::Zero;
		BlendOp _blendOpAlpha = BlendOp::Add;
		UINT8 _renderTargetWriteMask = 0xff;
	};

	struct BlendDesc
	{
		bool _alphaToCoverageEnable = false;
		bool _independentBlendEnable = false;
		BlendTargetDesc _renderTarget[8];

		Math::Color _blendFactor;
		uint32 _sampleMask = ~0u;
		bool _blendFactorDynamic = false;
	};

    enum class PrimitiveTopology
    {
        PointList,
        LineList,
        LineStrip,
        LineListAdjacency,
        LineStripAdjacency,
        TriangleList,
        TriangleStrip,
        TriangleListAdjacency,
        TriangleStripAdjacency,

		Patches1,
		Patches2,
		Patches3,
		Patches4,
		Patches5,
		Patches6,
		Patches7,
		Patches8,
		Patches9,
		Patches10,
		Patches11,
		Patches12,
		Patches13,
		Patches14,
		Patches15,
		Patches16,
		Patches17,
		Patches18,
		Patches19,
		Patches20,
		Patches21,
		Patches22,
		Patches23,
		Patches24,
		Patches25,
		Patches26,
		Patches27,
		Patches28,
		Patches29,
		Patches30,
		Patches31,
		Patches32,
    };

	class Shader;

	struct ShaderProgram
	{
		Shader* _vertexShader	= nullptr;
		Shader* _pixelShader	= nullptr;
		Shader* _geometryShader	= nullptr;
		Shader* _hullShader		= nullptr;
		Shader* _domainShader	= nullptr;
		Shader* _computeShader	= nullptr;
	};

    struct GraphicsPipelineDesc
    {
        const class PipelineLayout* _pipelineLayout = nullptr;

        ShaderProgram _shaderProgram;

        const class RenderPass* _renderPass = nullptr;

        PrimitiveTopology _primitiveTopology = PrimitiveTopology::TriangleList;

        std::vector<Viewport> _viewports;

        std::vector<Scissor> _scissors;

		bool _hasDSS = true;

        DepthDesc _depthDesc;

        StencilDesc _stencilDesc;

		bool _hasRRS = true;

        RasterizerDesc _rasterizerDesc;

		bool _hasBS = true;

		BlendDesc _blendDesc;
    };
}