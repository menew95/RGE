#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11CommandBuffer.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Buffer.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/DX11/DX11PipelineState.h"
#include "GraphicsModule/Core/DX11/DX11StateManager.h"

namespace Graphics
{
	namespace DX11
	{

		DX11CommandBuffer::DX11CommandBuffer(ID3D11Device* device, const ComPtr<ID3D11DeviceContext>& context, const std::shared_ptr<class DX11StateManager>& stateManager, const CommandBufferDesc& desc)
			: m_Device(device)
			, m_Context(context)
			, m_StateManager(stateManager)
		{
			if ((desc.flags & CommandBufferFlags::ImmediateSubmit) == 0)
			{
				m_IsDeferredContext = true;

				if ((desc.flags & CommandBufferFlags::MultiSubmit) != 0)
				{
					m_IsSecondaryCmdBuffer = true;
				}
			}
		}

		DX11CommandBuffer::~DX11CommandBuffer()
		{

		}

		void DX11CommandBuffer::Begin()
		{

		}

		void DX11CommandBuffer::End()
		{
			if (m_IsDeferredContext == true)
			{
				//m_Context->FinishCommandList( , deferredCommandBuffer);
			}
		}

		void DX11CommandBuffer::Execute(CommandBuffer& deferredCommandBuffer)
		{
			if (m_IsDeferredContext == true)
			{
				//m_Context->ExecuteCommandList(deferredCommandBuffer);
			}
			else
			{
				//m_Context->Draw(deferredCommandBuffer);
			}
		}

		void DX11CommandBuffer::Draw(uint32 numVertices, uint32 firstVertex)
		{
			m_Context->Draw(numVertices, firstVertex);
		}

		void DX11CommandBuffer::DrawIndexed(uint32 numIndices, uint32 firstIndex, uint32 offset /*= 0*/)
		{
			m_Context->DrawIndexed(numIndices, firstIndex, offset);
		}

		void DX11CommandBuffer::DrawInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 firstInstance)
		{
			m_Context->DrawInstanced(numVertices, firstVertex, numInstances, firstInstance);
		}

		void DX11CommandBuffer::DrawIndexedInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 vertexOffset, uint32 firstInstance)
		{
			m_Context->DrawIndexedInstanced(numVertices, firstVertex, numInstances, vertexOffset, firstInstance);
		}

		void DX11CommandBuffer::SetPipelineState(PipelineState& pipelineState)
		{
			auto& _pipelineState = reinterpret_cast<DX11PipelineState&>(pipelineState);

			_pipelineState.Bind(*m_StateManager);
		}

		void DX11CommandBuffer::SetVertexBuffer(Buffer& buffer)
		{
			auto& _buffer = reinterpret_cast<DX11Buffer&>(buffer);

			ID3D11Buffer* _buffers[] = { _buffer.GetBuffer() };
			uint32 _strides[] = { _buffer.GetStride() };
			uint32 _offsets[] = { 0 };

			m_Context->IASetVertexBuffers(0, 1, _buffers, _strides, _offsets);
		}

		void DX11CommandBuffer::SetIndexBuffer(Buffer& buffer)
		{
			auto& _buffer = reinterpret_cast<DX11Buffer&>(buffer);

			m_Context->IASetIndexBuffer(_buffer.GetBuffer(), _buffer.GetDXFormat(), 0);
		}

		void DX11CommandBuffer::UpdateBuffer(Buffer& dstBuffer, uint32 dstOffset, const void* data, uint32 dataSize)
		{
			auto& dstBufferD3D = reinterpret_cast<DX11Buffer&>(dstBuffer);

			dstBufferD3D.UpdateSubresource(m_Context.Get(), data, dataSize, dstOffset);
		}

		void DX11CommandBuffer::SetViewport(const Math::Viewport& viewport)
		{
			m_StateManager->SetViewports(1, &viewport);
		}

		void DX11CommandBuffer::SetViewports(uint32 numViewports, const Math::Viewport* viewport)
		{
			m_StateManager->SetViewports(numViewports, viewport);
		}

		void DX11CommandBuffer::SetScissor(const Math::Scissor& scissor)
		{
			m_StateManager->SetScissors(1, &scissor);
		}

		void DX11CommandBuffer::SetScissors(uint32 numScissors, const Math::Scissor* scissors)
		{
			m_StateManager->SetScissors(numScissors, scissors);
		}

		void DX11CommandBuffer::SetBuffer(Buffer* buffer, uint32 slot, uint32 bindFlags, uint32 stageFlags)
		{
			if ((bindFlags & (BindFlags::ShaderResource | BindFlags::UnorderedAccess)) != 0)
			{
				auto* _buffer = reinterpret_cast<DX11Buffer*>(buffer);

				if ((bindFlags & BindFlags::ConstantBuffer) != 0)
				{
					m_StateManager->SetConstantBuffers(slot, 1, _buffer->GetBufferRef(), stageFlags);
				}

				if ((bindFlags & BindFlags::ShaderResource) != 0)
				{
					assert(false);
					ID3D11ShaderResourceView* srv[] = { 0 };
					//m_StateManager->SetShaderResources(slot, 1, srv, stageFlags);
				}

				if ((bindFlags & BindFlags::UnorderedAccess) != 0)
				{
					assert(false);
					ID3D11UnorderedAccessView* uav[] = { 0 };
					UINT auvCounts[] = { 0 };
					//m_StateManager->SetUnorderedAccessViews(slot, 1, uav, auvCounts, stageFlags);
				}
			}
			else
			{
				auto* _buffer = reinterpret_cast<DX11Buffer*>(buffer);

				if ((bindFlags & BindFlags::ConstantBuffer) != 0)
				{
					m_StateManager->SetConstantBuffers(slot, 1, _buffer->GetBufferRef(), stageFlags);
				}
			}
		}

		void DX11CommandBuffer::SetBuffer()
		{

		}

		void DX11CommandBuffer::SetTexture(Texture* texture, uint32 slot, uint32 bindFlags, uint32 stageFlags)
		{
			auto* _texture = reinterpret_cast<DX11Texture*>(texture);

			if ((bindFlags & BindFlags::ShaderResource) != 0)
			{
				ID3D11ShaderResourceView* _srvs[] = { _texture->GetSRV() };
				m_StateManager->SetShaderResources(slot, 1, _srvs, stageFlags);
			}
		}

		void DX11CommandBuffer::SetSampler(Sampler* sampler, uint32 slot, uint32 stageFlags)
		{

		}

	}
}