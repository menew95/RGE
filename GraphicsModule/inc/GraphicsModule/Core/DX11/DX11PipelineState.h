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

		private:

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

			std::unique_ptr<char[]>			m_StaticStateBuffer;
			uint32							m_NumStaticViewports = 0;
			uint32							m_NumStaticScissors = 0;

		};
	}
}