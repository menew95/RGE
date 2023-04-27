#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11StateManager.h"
#include "GraphicsModule/Core/ShaderFlags.h"

#include "Math/Math.h"

namespace Graphics
{
	namespace DX11
	{
		static constexpr bool IsCompatibleToD3DViewport()
		{
			using namespace Math;

			return
				(
					sizeof(D3D11_VIEWPORT) == sizeof(Viewport) &&
					offsetof(D3D11_VIEWPORT, TopLeftX) == offsetof(Viewport, x) &&
					offsetof(D3D11_VIEWPORT, TopLeftY) == offsetof(Viewport, y) &&
					offsetof(D3D11_VIEWPORT, Width) == offsetof(Viewport, width) &&
					offsetof(D3D11_VIEWPORT, Height) == offsetof(Viewport, height) &&
					offsetof(D3D11_VIEWPORT, MinDepth) == offsetof(Viewport, minDepth) &&
					offsetof(D3D11_VIEWPORT, MaxDepth) == offsetof(Viewport, maxDepth)
					);
		}

		DX11StateManager::DX11StateManager(ID3D11Device* device, const ComPtr<ID3D11DeviceContext> context)
			: m_Context(context)
		{

		}

		void DX11StateManager::SetViewports(uint32 numViewports, const Math::Viewport* viewportArray)
		{
			numViewports = std::min(numViewports, std::uint32_t(D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE));

			if (IsCompatibleToD3DViewport())
			{
				m_Context->RSSetViewports(numViewports, reinterpret_cast<const D3D11_VIEWPORT*>(viewportArray));
			}
			else
			{
				D3D11_VIEWPORT viewportsD3D[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

				for (std::uint32_t i = 0; i < numViewports; ++i)
				{
					const auto& src = viewportArray[i];
					auto& dst = viewportsD3D[i];

					dst.TopLeftX = src.x;
					dst.TopLeftY = src.y;
					dst.Width = src.width;
					dst.Height = src.height;
					dst.MinDepth = src.minDepth;
					dst.MaxDepth = src.maxDepth;
				}

				m_Context->RSSetViewports(numViewports, viewportsD3D);
			}
		}

		void DX11StateManager::SetScissors(uint32 numScissors, const Math::Scissor* scissorArray)
		{
			numScissors = std::min(numScissors, std::uint32_t(D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE));

			D3D11_RECT scissorsD3D[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

			for (std::uint32_t i = 0; i < numScissors; ++i)
			{
				const auto& src = scissorArray[i];
				auto& dst = scissorsD3D[i];

				dst.left = src.x;
				dst.top = src.y;
				dst.right = src.x + src.width;
				dst.bottom = src.y + src.height;
			}

			m_Context->RSSetScissorRects(numScissors, scissorsD3D);
		}

		void DX11StateManager::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
		{
			if (m_PrimitiveTopology != primitiveTopology)
			{
				m_PrimitiveTopology = primitiveTopology;

				m_Context->IASetPrimitiveTopology(primitiveTopology);
			}
		}

		void DX11StateManager::SetInputLayout(ID3D11InputLayout* inputLayout)
		{
			if (m_InputLayout != inputLayout)
			{
				m_InputLayout = inputLayout;

				m_Context->IASetInputLayout(inputLayout);
			}
		}

		void DX11StateManager::SetVertexShader(ID3D11VertexShader* shader)
		{
			if (m_VS != shader)
			{
				m_VS = shader;

				m_Context->VSSetShader(shader, nullptr, 0);
			}
		}

		void DX11StateManager::SetHullShader(ID3D11HullShader* shader)
		{
			if (m_HS != shader)
			{
				m_HS = shader;

				m_Context->HSSetShader(shader, nullptr, 0);
			}
		}

		void DX11StateManager::SetDomainShader(ID3D11DomainShader* shader)
		{
			if (m_DS != shader)
			{
				m_DS = shader;

				m_Context->DSSetShader(shader, nullptr, 0);
			}
		}

		void DX11StateManager::SetGeometryShader(ID3D11GeometryShader* shader)
		{
			if (m_GS != shader)
			{
				m_GS = shader;

				m_Context->GSSetShader(shader, nullptr, 0);
			}
		}

		void DX11StateManager::SetPixelShader(ID3D11PixelShader* shader)
		{
			if (m_PS != shader)
			{
				m_PS = shader;

				m_Context->PSSetShader(shader, nullptr, 0);
			}
		}

		void DX11StateManager::SetComputeShader(ID3D11ComputeShader* shader)
		{
			if (m_CS != shader)
			{
				m_CS = shader;

				m_Context->CSSetShader(shader, nullptr, 0);
			}
		}

		void DX11StateManager::SetRasterizerState(ID3D11RasterizerState* rasterizerState)
		{
			if (m_RasterizerState != rasterizerState)
			{
				m_RasterizerState = rasterizerState;

				m_Context->RSSetState(rasterizerState);
			}
		}

		void DX11StateManager::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState)
		{
			if (m_DepthStencilState != depthStencilState)
			{
				m_DepthStencilState = depthStencilState;

				m_Context->OMSetDepthStencilState(depthStencilState, m_StencilRef);
			}
		}

		void DX11StateManager::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState, uint32 stencilRef)
		{
			if (m_DepthStencilState != depthStencilState || m_StencilRef != stencilRef)
			{
				m_DepthStencilState = depthStencilState;

				m_StencilRef = stencilRef;

				m_Context->OMSetDepthStencilState(depthStencilState, stencilRef);
			}
		}

		void DX11StateManager::SetStencilRef(uint32 stencilRef)
		{
			if (m_StencilRef != stencilRef)
			{
				m_StencilRef = stencilRef;

				m_Context->OMSetDepthStencilState(m_DepthStencilState, stencilRef);
			}
		}

		void DX11StateManager::SetBlendState(ID3D11BlendState* blendState, uint32 sampleMask)
		{
			if (m_BlendState != blendState)
			{
				m_BlendState = blendState;

				m_Context->OMSetBlendState(blendState, &m_BlendFactor[0], sampleMask);
			}
		}

		void DX11StateManager::SetBlendState(ID3D11BlendState* blendState, const FLOAT* blendFactor, uint32 sampleMask)
		{
			if (m_BlendState != blendState 
				|| m_BlendFactor[0] != blendFactor[0]
				|| m_BlendFactor[1] != blendFactor[1]
				|| m_BlendFactor[2] != blendFactor[2]
				|| m_BlendFactor[3] != blendFactor[3])
			{
				m_BlendState = blendState;

				m_BlendFactor[0] = blendFactor[0];
				m_BlendFactor[1] = blendFactor[1];
				m_BlendFactor[2] = blendFactor[2];
				m_BlendFactor[3] = blendFactor[3];

				m_Context->OMSetBlendState(blendState, &m_BlendFactor[0], sampleMask);
			}
		}

		void DX11StateManager::SetBlendFactor(const FLOAT* blendFactor)
		{
			m_BlendFactor[0] = blendFactor[0];
			m_BlendFactor[1] = blendFactor[1];
			m_BlendFactor[2] = blendFactor[2];
			m_BlendFactor[3] = blendFactor[3];
		}

		void DX11StateManager::SetConstantBuffers(uint32 startSlot, uint32 count, ID3D11Buffer* const* buffers, long stageFlags)
		{
			if ((stageFlags & StageFlags::VS) != 0)
			{
				m_Context->VSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::HS) != 0)
			{
				m_Context->HSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::DS) != 0)
			{
				m_Context->DSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::GS) != 0)
			{
				m_Context->GSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::PS) != 0)
			{
				m_Context->PSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::CS) != 0)
			{
				m_Context->CSSetConstantBuffers(startSlot, count, buffers);
			}
		}

		void DX11StateManager::SetConstantBuffersRange(uint32 startSlot, uint32 count, ID3D11Buffer* const* buffers, const uint32* firstConstants, const uint32* numConstants, long stageFlags)
		{
			for (uint32 i = 0; i < count; i++)
			{
				if (firstConstants[i] > 0)
				{
					throw std::runtime_error("constant buffer range is only supported with Direct3D 11.1 or later");
				}
			}

			if ((stageFlags & StageFlags::VS) != 0)
			{
				m_Context->VSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::HS) != 0)
			{
				m_Context->HSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::DS) != 0)
			{
				m_Context->DSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::GS) != 0)
			{
				m_Context->GSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::PS) != 0)
			{
				m_Context->PSSetConstantBuffers(startSlot, count, buffers);
			}
			if ((stageFlags & StageFlags::CS) != 0)
			{
				m_Context->CSSetConstantBuffers(startSlot, count, buffers);
			}
		}

		void DX11StateManager::SetShaderResources(uint32 startSlot, uint32 count, ID3D11ShaderResourceView* const* views, long stageFlags)
		{
			if ((stageFlags & StageFlags::VS) != 0)
			{
				m_Context->VSSetShaderResources(startSlot, count, views);
			}
			if ((stageFlags & StageFlags::HS) != 0)
			{
				m_Context->HSSetShaderResources(startSlot, count, views);
			}
			if ((stageFlags & StageFlags::DS) != 0)
			{
				m_Context->DSSetShaderResources(startSlot, count, views);
			}
			if ((stageFlags & StageFlags::GS) != 0)
			{
				m_Context->GSSetShaderResources(startSlot, count, views);
			}
			if ((stageFlags & StageFlags::PS) != 0)
			{
				m_Context->PSSetShaderResources(startSlot, count, views);
			}
			if ((stageFlags & StageFlags::CS) != 0)
			{
				m_Context->CSSetShaderResources(startSlot, count, views);
			}
		}

		void DX11StateManager::SetSamplers(uint32 startSlot, uint32 count, ID3D11SamplerState* const* samplers, long stageFlags)
		{
			if ((stageFlags & StageFlags::VS) != 0)
			{
				m_Context->VSSetSamplers(startSlot, count, samplers);
			}
			if ((stageFlags & StageFlags::HS) != 0)
			{
				m_Context->HSSetSamplers(startSlot, count, samplers);
			}
			if ((stageFlags & StageFlags::DS) != 0)
			{
				m_Context->DSSetSamplers(startSlot, count, samplers);
			}
			if ((stageFlags & StageFlags::GS) != 0)
			{
				m_Context->GSSetSamplers(startSlot, count, samplers);
			}
			if ((stageFlags & StageFlags::PS) != 0)
			{
				m_Context->PSSetSamplers(startSlot, count, samplers);
			}
			if ((stageFlags & StageFlags::CS) != 0)
			{
				m_Context->CSSetSamplers(startSlot, count, samplers);
			}
		}

		void DX11StateManager::SetUnorderedAccessViews(uint32 startSlot, uint32 count, ID3D11UnorderedAccessView* const* views, const uint32* initialCounts, long stageFlags)
		{
			if ((stageFlags & StageFlags::PS) != 0)
			{
				m_Context->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, nullptr, nullptr, startSlot, count, views, initialCounts);
			}
			if ((stageFlags & StageFlags::CS) != 0)
			{
				m_Context->CSSetUnorderedAccessViews(startSlot, count, views, initialCounts);
			}
		}

		void DX11StateManager::ClearState()
		{
			m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			m_InputLayout = nullptr;

			m_VS = nullptr;
			m_HS = nullptr;
			m_DS = nullptr;
			m_GS = nullptr;
			m_PS = nullptr;
			m_CS = nullptr;

			m_RasterizerState = nullptr;
			m_DepthStencilState = nullptr;
			m_StencilRef = 0;
			m_BlendState = nullptr;
			m_SampleMask = 0xffffffff;
		}

	}
}