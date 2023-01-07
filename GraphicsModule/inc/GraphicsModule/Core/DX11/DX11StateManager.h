#pragma once

#include "GraphicsModule/Core/StateManager.h"
#include "GraphicsModule/Core/Common/ComPtr.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Math
{
	class Viewport;
	struct Scissor;
}

namespace Graphics
{
	namespace DX11
	{
		class DX11StateManager final : public StateManager
		{
		public:
			DX11StateManager(ID3D11Device* device,
				const ComPtr<ID3D11DeviceContext> context);
			//~DX11StateManager() override;

			void SetViewports(uint32 numViewports, const Math::Viewport* viewportArray);
			void SetScissors(uint32 numScissors, const Math::Scissor* scissorArray);

			void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY primitiveTopology);
			void SetInputLayout(ID3D11InputLayout* inputLayout);

			void SetVertexShader(ID3D11VertexShader* shader);
			void SetHullShader(ID3D11HullShader* shader);
			void SetDomainShader(ID3D11DomainShader* shader);
			void SetGeometryShader(ID3D11GeometryShader* shader);
			void SetPixelShader(ID3D11PixelShader* shader);
			void SetComputeShader(ID3D11ComputeShader* shader);

			void SetRasterizerState(ID3D11RasterizerState* rasterizerState);

			void SetDepthStencilState(ID3D11DepthStencilState* depthStencilState);
			void SetDepthStencilState(ID3D11DepthStencilState* depthStencilState, uint32 stencilRef);
			void SetStencilRef(uint32 stencilRef);

			void SetBlendState(ID3D11BlendState* blendState, uint32 sampleMask);
			void SetBlendState(ID3D11BlendState* blendState, const FLOAT* blendFactor, uint32 sampleMask);
			void SetBlendFactor(const FLOAT* blendFactor);

			void SetConstantBuffers(
				uint32                    startSlot,
				uint32                    count,
				ID3D11Buffer* const* buffers,
				long                    stageFlags
			);

			void SetConstantBuffersRange(
				uint32                    startSlot,
				uint32                    count,
				ID3D11Buffer* const* buffers,
				const uint32* firstConstants,
				const uint32* numConstants,
				long                    stageFlags
			);

			void SetShaderResources(
				uint32                                startSlot,
				uint32                                count,
				ID3D11ShaderResourceView* const* views,
				long                                stageFlags
			);

			void SetSamplers(
				uint32                        startSlot,
				uint32                        count,
				ID3D11SamplerState* const* samplers,
				long                        stageFlags
			);

			void SetUnorderedAccessViews(
				uint32                                startSlot,
				uint32                                count,
				ID3D11UnorderedAccessView* const* views,
				const uint32* initialCounts,
				long                                stageFlags
			);

			inline ID3D11DeviceContext* GetContext() const
			{
				return m_Context.Get();
			}

		private:
			ComPtr<ID3D11DeviceContext> m_Context;

			D3D11_PRIMITIVE_TOPOLOGY    m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			ID3D11InputLayout* m_InputLayout = nullptr;

			ID3D11VertexShader* m_VS = nullptr;
			ID3D11HullShader* m_HS = nullptr;
			ID3D11DomainShader* m_DS = nullptr;
			ID3D11GeometryShader* m_GS = nullptr;
			ID3D11PixelShader* m_PS = nullptr;
			ID3D11ComputeShader* m_CS = nullptr;

			ID3D11RasterizerState* m_RasterizerState = nullptr;
			ID3D11DepthStencilState* m_DepthStencilState = nullptr;
			uint32 m_StencilRef = 0;
			ID3D11BlendState* m_BlendState = nullptr;
			float m_BlendFactor[4] = { -1.0f, -1.0f, -1.0f, -1.0f };
			uint32 m_SampleMask = 0xffffffff;
		};
	}
}