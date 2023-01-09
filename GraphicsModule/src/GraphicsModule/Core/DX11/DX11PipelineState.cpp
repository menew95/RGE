#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11PipeLineState.h"
#include "GraphicsModule/Core/DX11/DX11StateManager.h"
#include "GraphicsModule/Core/DX11/DX11Type.h"

namespace Graphics
{
	namespace DX11
	{

		DX11PipelineState::DX11PipelineState(ID3D11Device* device, const GraphicsPipelineDesc& desc)
		{
			m_PrimitiveTopology = MapPrimitive(desc._primitiveTopology);
			//m_StencilRefDynamic = desc._stencilDesc.
		}

		void DX11PipelineState::Bind(DX11StateManager& stateManager)
		{

			/* Set input-assembly states */
			stateManager.SetPrimitiveTopology(m_PrimitiveTopology);
			stateManager.SetInputLayout(m_InputLayout.Get());

			/* Set shader states */
			stateManager.SetVertexShader(m_VS.Get());
			stateManager.SetHullShader(m_HS.Get());
			stateManager.SetDomainShader(m_DS.Get());
			stateManager.SetGeometryShader(m_GS.Get());
			stateManager.SetPixelShader(m_PS.Get());
			stateManager.SetComputeShader(m_CS.Get());

			//stateManager.SetViewports();


			///* Bind rasterizer state */
			//stateMngr.SetRasterizerState(rasterizerState_.Get());

			///* Bind depth-stencil state */
			//if (IsStencilRefDynamic())
			//	stateMngr.SetDepthStencilState(depthStencilState_.Get());
			//else
			//	stateMngr.SetDepthStencilState(depthStencilState_.Get(), GetStencilRef());

			///* Bind blend state */
			//if (IsBlendFactorDynamic())
			//	stateMngr.SetBlendState(blendState_.Get(), GetSampleMask());
			//else
			//	stateMngr.SetBlendState(blendState_.Get(), GetBlendFactor(), GetSampleMask());
		}

	}
}