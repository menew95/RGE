#include "GraphicsPCH.h"

#include "GraphicsModule/Core/DX11/DX11CommandBuffer.h"
#include "GraphicsModule/Core/DX11/DX11PipelineState.h"
#include "GraphicsModule/Core/DX11/DX11PipelineLayout.h"
#include "GraphicsModule/Core/DX11/DX11StateManager.h"
#include "GraphicsModule/Core/DX11/DX11RenderTarget.h"
#include "GraphicsModule/Core/DX11/DX11RenderPass.h"
#include "GraphicsModule/Core/DX11/DX11SwapChain.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/DX11/DX11Sampler.h"
#include "GraphicsModule/Core/DX11/DX11Buffer.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"


namespace Graphics
{
	namespace DX11
	{
		// Global array of null pointers to unbind resource slots
		static void* const	g_nullResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
		static void* const	g_nullRTVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
		static const uint32	g_zeroCounters[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};

		DX11CommandBuffer::DX11CommandBuffer(ID3D11Device* device, const ComPtr<ID3D11DeviceContext>& context, const std::shared_ptr<class DX11StateManager>& stateManager, const CommandBufferDesc& desc)
			: m_Device(device)
			, m_Context(context)
			, m_StateManager(stateManager)
			//, m_DepthStencilView(nullptr)
		{
			if ((desc._flags & CommandBufferFlags::ImmediateSubmit) == 0)
			{
				m_IsDeferredContext = true;

				if ((desc._flags & CommandBufferFlags::MultiSubmit) != 0)
				{
					m_IsSecondaryCmdBuffer = true;
				}
			}

			HRESULT _hr = m_Context->QueryInterface(__uuidof(m_User), &m_User);

			HR(_hr, "failed to get d3d11-user-defined-annotation");
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

		void DX11CommandBuffer::UpdateBuffer(Buffer& dstBuffer, uint32 dstOffset, const void* data, uint32 dataSize)
		{
			auto& dstBufferD3D = reinterpret_cast<DX11Buffer&>(dstBuffer);

			dstBufferD3D.UpdateSubresource(m_Context.Get(), data, dataSize, dstOffset);
		}

		void DX11CommandBuffer::CopyBuffer(Buffer& dstBuffer, uint32 dstOffset, Buffer& srcBuffer, uint32 srcOffset, uint32 size)
		{

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

		void DX11CommandBuffer::SetResource(Resource& resource, uint32 slot, long bindFlags, long stageFlags /*= StageFlags::AllStages*/)
		{
			switch (resource.GetResourceType())
			{
				case ResourceType::Undefined:
					break;
				case ResourceType::Buffer:
				{
					auto& _buffer = reinterpret_cast<Buffer&>(resource);
					SetBuffer(_buffer, slot, bindFlags, stageFlags);
					break;
				}
				case ResourceType::Texture:
				{
					auto& _texture = reinterpret_cast<Texture&>(resource);
					SetTexture(_texture, slot, bindFlags, stageFlags);
					break;
				}
				case ResourceType::Sampler:
				{
					auto& _sampler = reinterpret_cast<Sampler&>(resource);
					SetSampler(_sampler, slot, stageFlags);
					break;
				}
			}
		}

		void DX11CommandBuffer::ResetResourceSlots(const ResourceType resourceType, uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags /*= StageFlags::AllStages*/)
		{
			if (numSlots > 0)
			{
				switch (resourceType)
				{
					case ResourceType::Undefined:
						break;
					case ResourceType::Buffer:
						ResetBufferSlots(firstSlot, numSlots, bindFlags, stageFlags);
						break;
					case ResourceType::Texture:
						ResetTextureSlots(firstSlot, numSlots, bindFlags, stageFlags);
						break;
					case ResourceType::Sampler:
						ResetSamplerSlots(firstSlot, numSlots, stageFlags);
						break;
				}
			}
		}

		void DX11CommandBuffer::BeginRenderPass(RenderPass& renderPass, uint32 numClearValues, const ClearValue* clearValues)
		{
			auto& _castPass = reinterpret_cast<DX11RenderPass&>(renderPass);

		}

		void DX11CommandBuffer::EndRenderPass()
		{
			m_Context->OMSetRenderTargets(m_FramebufferView._numRenderTargetViews, 
				reinterpret_cast<ID3D11RenderTargetView* const*>(g_nullRTVs), nullptr);

			::memset(&m_FramebufferView, 0, sizeof(D3D11FramebufferView));
		}

		void DX11CommandBuffer::Clear(long flags, const ClearValue& clearValue /*= {}*/)
		{
			/* Clear color buffers */
			if ((flags & ClearFlags::Color) != 0)
			{
				for (UINT i = 0; i < m_FramebufferView._numRenderTargetViews; ++i)
					m_Context->ClearRenderTargetView(m_FramebufferView._renderTargetViews[i], &clearValue._color.x);
			}

			/* Clear depth-stencil buffer */
			if (m_FramebufferView._depthStencilView != nullptr)
			{
				uint32 _flags = 0;

				if ((flags & ClearFlags::Depth) != 0)
					_flags |= D3D11_CLEAR_DEPTH;
				if ((flags & ClearFlags::Stencil) != 0)
					_flags |= D3D11_CLEAR_STENCIL;
				
				m_Context->ClearDepthStencilView(
					m_FramebufferView._depthStencilView,
					_flags,
					clearValue._depth,
					static_cast<UINT8>(clearValue._stencil & 0xff)
				);
			}
		}

		void DX11CommandBuffer::ClearAttachments(uint32 numAttachments, const AttachmentClear* attachments)
		{
			for (uint32 i = 0; i < numAttachments; i++)
			{
				if ((attachments[i]._flags & ClearFlags::Color) != 0)
				{

					m_Context->ClearRenderTargetView(
						m_FramebufferView._renderTargetViews[attachments[i]._colorAttachment],
						attachments[i]._clearValue._color
					);
				}
				else if (m_FramebufferView._depthStencilView != nullptr)
				{
					uint32 _flags = 0;

					if ((attachments[i]._flags & ClearFlags::Depth) != 0)
						_flags |= D3D11_CLEAR_DEPTH;
					if ((attachments[i]._flags & ClearFlags::Stencil) != 0)
						_flags |= D3D11_CLEAR_STENCIL;

					m_Context->ClearDepthStencilView(
						m_FramebufferView._depthStencilView,
						_flags, attachments->_clearValue._depth,
						static_cast<UINT8>(attachments->_clearValue._stencil & 0xff));
				}
			}
		}

		void DX11CommandBuffer::SetRenderTarget(RenderTarget& renderTarget, uint32 numAttachments, const AttachmentClear* attachments)
		{
			if (renderTarget.GetType() == RenderTarget::Type::SwapChain)
			{
				SwapChain* _swapChain = reinterpret_cast<SwapChain*>(&renderTarget);
				BindSwapChain(_swapChain);

				ClearAttachments(numAttachments, attachments);
			}
			else
			{
				auto& _renderTarget = reinterpret_cast<DX11RenderTarget&>(renderTarget);

				_renderTarget.ClearRenderTarget(m_Context.Get(), numAttachments, attachments);

				auto _numRTV = static_cast<uint32>(_renderTarget.GetRenderTargetViews().size());

				m_Context->OMSetRenderTargets(
					_numRTV,
					_renderTarget.GetRenderTargetViews().data(),
					_renderTarget.GetDepthStencilView()
				);

				m_FramebufferView._numRenderTargetViews = _numRTV;
				m_FramebufferView._renderTargetViews = _renderTarget.GetRenderTargetViews().data();
				m_FramebufferView._depthStencilView = _renderTarget.GetDepthStencilView();
			}
		}

		void DX11CommandBuffer::SetPipelineState(PipelineState& pipelineState)
		{
			auto& _pipelineState = reinterpret_cast<DX11PipelineState&>(pipelineState);

			_pipelineState.Bind(*m_StateManager);
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

		void DX11CommandBuffer::SetBuffer(Buffer& buffer, uint32 slot, uint32 bindFlags, uint32 stageFlags)
		{
			/*if ((bindFlags & (BindFlags::ShaderResource | BindFlags::UnorderedAccess)) != 0)
			{
				auto _buffer = reinterpret_cast<DX11Buffer&>(buffer);

				if ((bindFlags & BindFlags::ConstantBuffer) != 0)
				{
					m_StateManager->SetConstantBuffers(slot, 1, _buffer.GetBufferRef(), stageFlags);
				}

				if ((bindFlags & BindFlags::ShaderResource) != 0)
				{
					assert(false);
					ID3D11ShaderResourceView* srv[] = { 0 };
					m_StateManager->SetShaderResources(slot, 1, srv, stageFlags);
				}

				if ((bindFlags & BindFlags::UnorderedAccess) != 0)
				{
					assert(false);
					ID3D11UnorderedAccessView* uav[] = { 0 };
					UINT auvCounts[] = { 0 };
					m_StateManager->SetUnorderedAccessViews(slot, 1, uav, auvCounts, stageFlags);
				}
			}
			else
			{*/
				auto _buffer = reinterpret_cast<DX11Buffer&>(buffer);

				if ((bindFlags & BindFlags::ConstantBuffer) != 0)
				{
					m_StateManager->SetConstantBuffers(slot, 1, _buffer.GetBufferRef(), stageFlags);
				}
			//}
		}

		void DX11CommandBuffer::SetTexture(Texture& texture, uint32 slot, uint32 bindFlags, uint32 stageFlags)
		{
			auto _texture = reinterpret_cast<DX11Texture&>(texture);

			if ((bindFlags & BindFlags::ShaderResource) != 0)
			{
				ID3D11ShaderResourceView* _srvs[] = { _texture.GetSRV() };
				m_StateManager->SetShaderResources(slot, 1, _srvs, stageFlags);
			}

			if ((bindFlags & BindFlags::UnorderedAccess) != 0)
			{
				ID3D11UnorderedAccessView* uav[] = { _texture.GetUAV() };
				UINT auvCounts[] = { 0 };
				m_StateManager->SetUnorderedAccessViews(slot, 1, uav, auvCounts, stageFlags);
			}
		}

		void DX11CommandBuffer::SetSampler(Sampler& sampler, uint32 slot, uint32 stageFlags)
		{
			auto& _sampler = reinterpret_cast<DX11Sampler&>(sampler);

			ID3D11SamplerState* samplerStates[] = { _sampler.GetSamplerState() };
			m_StateManager->SetSamplers(slot, 1, samplerStates, stageFlags);
		}

		void DX11CommandBuffer::Dispatch(uint32 numWorkGroupsX, uint32 numWorkGroupsY, uint32 numWorkGroupsZ)
		{
			// Todo : compute shader
			assert(false);
		}

		void DX11CommandBuffer::ResetBufferSlots(uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags /*= StageFlags::AllStages*/)
		{
			if ((bindFlags & BindFlags::VertexBuffer) != 0)
			{
				if ((stageFlags & StageFlags::VS) != 0)
				{
					/* Clamp slot indices */
					firstSlot = std::min(firstSlot, std::uint32_t(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT) - 1);
					numSlots = std::min(numSlots, std::uint32_t(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT) - firstSlot);

					/* Unbind vertex buffers */
					m_Context->IASetVertexBuffers(
						firstSlot,
						numSlots,
						reinterpret_cast<ID3D11Buffer* const*>(g_nullResources),
						g_zeroCounters,
						g_zeroCounters
					);
				}
			}

			if ((bindFlags & BindFlags::IndexBuffer) != 0)
			{
				if (firstSlot == 0 && (stageFlags & StageFlags::VS) != 0)
					m_Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0);
			}

			if ((bindFlags & BindFlags::ConstantBuffer) != 0)
			{
				/* Clamp slot indices */
				firstSlot = std::min(firstSlot, std::uint32_t(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) - 1);
				numSlots = std::min(numSlots, std::uint32_t(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) - firstSlot);

				m_StateManager->SetConstantBuffers(
					firstSlot,
					numSlots,
					reinterpret_cast<ID3D11Buffer* const*>(g_nullResources),
					stageFlags);
			}

			if ((bindFlags & BindFlags::StreamOutput) != 0)
			{
				if (firstSlot == 0 && (stageFlags & (StageFlags::VS | StageFlags::GS)) != 0)
				{
					/* Clamp slot indices */
					firstSlot = std::min(firstSlot, std::uint32_t(D3D11_SO_BUFFER_SLOT_COUNT) - 1);
					numSlots = std::min(numSlots, std::uint32_t(D3D11_SO_BUFFER_SLOT_COUNT) - firstSlot);

					/* Unbind stream-output buffers */
					m_Context->SOSetTargets(
						numSlots,
						reinterpret_cast<ID3D11Buffer* const*>(g_nullResources),
						g_zeroCounters
					);
				}
			}
		}

		void DX11CommandBuffer::ResetTextureSlots(uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags /*= StageFlags::AllStages*/)
		{
			if ((bindFlags & BindFlags::ShaderResource) != 0)
			{
				ResetResourceSlotsSRV(firstSlot, numSlots, stageFlags);
			}

			if ((bindFlags & BindFlags::UnorderedAccess) != 0)
			{
				ResetResourceSlotsUAV(firstSlot, numSlots, stageFlags);
			}
		}

		void DX11CommandBuffer::ResetSamplerSlots(uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags /*= StageFlags::AllStages*/)
		{
			firstSlot = std::min(firstSlot, std::uint32_t(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT) - 1);
			numSlots = std::min(numSlots, std::uint32_t(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT) - firstSlot);

			m_StateManager->SetSamplers(firstSlot, numSlots, reinterpret_cast<ID3D11SamplerState* const*>(g_nullResources), stageFlags);
		}

		void DX11CommandBuffer::ResetResourceSlotsSRV(uint32 firstSlot, uint32 numSlots, long stageFlags)
		{
			firstSlot = std::min(firstSlot, std::uint32_t(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) - 1);
			numSlots = std::min(numSlots, std::uint32_t(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) - firstSlot);

			m_StateManager->SetShaderResources(
				firstSlot,
				numSlots,
				reinterpret_cast<ID3D11ShaderResourceView* const*>(g_nullResources),
				stageFlags
			);
		}

		void DX11CommandBuffer::ResetResourceSlotsUAV(uint32 firstSlot, uint32 numSlots, long stageFlags)
		{
			firstSlot = std::min(firstSlot, std::uint32_t(D3D11_1_UAV_SLOT_COUNT) - 1);
			numSlots = std::min(numSlots, std::uint32_t(D3D11_1_UAV_SLOT_COUNT) - firstSlot);

			m_StateManager->SetUnorderedAccessViews(
				firstSlot,
				numSlots,
				reinterpret_cast<ID3D11UnorderedAccessView* const*>(g_nullResources),
				nullptr,
				stageFlags
			);
		}

		void DX11CommandBuffer::SetResources(PipelineLayout& pipelineLayout)
		{
			DX11PipelineLayout& _castLayout = reinterpret_cast<DX11PipelineLayout&>(pipelineLayout);

			auto& _binds = _castLayout.GetBindings();
			auto& _resources = _castLayout.GetResources();

			for (uint32 i = 0; i < pipelineLayout.GetNumBindings(); i++)
			{
				SetResource(*_resources[i], _binds[i]._slot, _binds[i]._bindFlags, _binds[i]._stageFlags);
			}
		}

		void DX11CommandBuffer::BeginEvent(const wchar_t* event)
		{
			m_User->BeginEvent(event);
		}

		void DX11CommandBuffer::EndEvent()
		{
			m_User->EndEvent();
		}

		void DX11CommandBuffer::BindFramebufferView(uint32 numRTVs, ID3D11RenderTargetView* const* renderTargetViews, ID3D11DepthStencilView* depthStencilView)
		{
			m_Context->OMSetRenderTargets(numRTVs, renderTargetViews, depthStencilView);

			m_FramebufferView._numRenderTargetViews = numRTVs;
			m_FramebufferView._renderTargetViews = renderTargetViews;
			m_FramebufferView._depthStencilView = depthStencilView;
		}

		void DX11CommandBuffer::BindSwapChain(SwapChain* swapChain)
		{
			DX11SwapChain* _castSwapChain = reinterpret_cast<DX11SwapChain*>(swapChain);

			_castSwapChain->BindFramebufferView(this);
		}

	}
}