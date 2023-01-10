#pragma once

#include "GraphicsModule/Core/PipelineState.h"
#include "GraphicsModule/Core/Common/ComPtr.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Graphics
{
	namespace DX11
	{

		class DX11StateManager;

		class DX11PipelineState : public PipelineState
		{

		public:
			DX11PipelineState(ID3D11Device* device, const GraphicsPipelineDesc& desc);

			virtual void Bind(DX11StateManager& stateMngr);

			void SetStaticViewportsAndScissors(DX11StateManager& stateManager);
			
		protected:
			
			inline D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const
			{
				return m_PrimitiveTopology;
			}
			inline UINT GetStencilRef() const
			{
				return m_StencilRef;
			}
			inline bool IsStencilRefDynamic() const
			{
				return m_StencilRefDynamic;
			}
			inline const FLOAT* GetBlendFactor() const
			{
				return m_BlendFactor;
			}
			inline bool IsBlendFactorDynamic() const
			{
				return m_BlendFactorDynamic;
			}

			inline UINT GetSampleMask() const
			{
				return m_SampleMask;
			}


		private:
			void GetDXDepthStencil(D3D11_DEPTH_STENCIL_DESC& desc, const DepthDesc& depthDesc, const StencilDesc stencilDesc);
			void GetDXRasterizer(D3D11_RASTERIZER_DESC& desc, const RasterizerDesc& rasterizerDesc);
			void GetDXBlend(D3D11_BLEND_DESC& desc, const BlendDesc& blendDesc);

			void CreateDepthStencilState(ID3D11Device* device, const GraphicsPipelineDesc& pipelineDesc);

			void CreateRasterizerState(ID3D11Device* device, const GraphicsPipelineDesc& pipelineDesc);

			void CreateBlendState(ID3D11Device* device, const GraphicsPipelineDesc& pipelineDesc);

			void GetShaderObjects(const ShaderProgram& shaderProgram);


			ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
			ComPtr<ID3D11RasterizerState>   m_RasterizerState;
			ComPtr<ID3D11BlendState>        m_BlendState;

			ComPtr<ID3D11InputLayout>       m_InputLayout;

			ComPtr<ID3D11VertexShader>		m_VS;
			ComPtr<ID3D11HullShader>		m_HS;
			ComPtr<ID3D11DomainShader>		m_DS;
			ComPtr<ID3D11GeometryShader>	m_GS;
			ComPtr<ID3D11PixelShader>		m_PS;
			ComPtr<ID3D11ComputeShader>		m_CS;

			D3D11_PRIMITIVE_TOPOLOGY        m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
			bool                            m_StencilRefDynamic = false;
			uint32                          m_StencilRef = 0;
			bool                            m_BlendFactorDynamic = false;
			float                           m_BlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			uint32                          m_SampleMask = UINT_MAX;

			std::vector<Math::Viewport>		m_Viewports;
			std::vector<Math::Scissor>		m_Scissors;

		};
	}
}