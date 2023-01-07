#pragma once

#include "GraphicsModule/Core/Device.h"
#include "GraphicsModule/Core/DeviceContext.h"

#include "GraphicsModule/Core/Buffer.h"
#include "GraphicsModule/Core/Texture.h"
#include "GraphicsModule/Core/Sampler.h"
#include "GraphicsModule/Core/CommandBuffer.h"
#include "GraphicsModule/Core/ResourceFlags.h"

#include "GraphicsModule/Core/Shader.h"
#include "GraphicsModule/Core/SwapChain.h"
#include "GraphicsModule/Core/RenderPass.h"
#include "GraphicsModule/Core/RenderTarget.h"
#include "GraphicsModule/Core/PipelineLayout.h"
#include "GraphicsModule/Core/PipelineState.h"

namespace Graphics
{
	class RenderSystem
	{
	public:
		RenderSystem();
		virtual ~RenderSystem();

		/* ----- SwapChain ----- */
		virtual SwapChain* CreateSwapChain(const SwapChainDesc& desc) abstract;
		virtual void Release(SwapChain& swapChain) abstract;

		/* ----- Command Buffer ----- */
		virtual CommandBuffer* CreateCommandBuffer(const CommandBufferDesc& desc) abstract;
		virtual void Release(CommandBuffer& commandBuffer) abstract;

		/* ----- Buffer ----- */
		virtual Buffer* CreateBuffer(const BufferDesc& desc, const void* initData = nullptr) abstract;
		virtual void Release(Buffer& buffer) abstract;

		/* ----- Textures ----- */
		virtual Texture* CreateTexture(const TextureDesc& desc, const class SrcImageDesc* imageDesc = nullptr) abstract;
		virtual void Release(Texture& texture) abstract;

		/* ----- Samplers ---- */
		virtual Sampler* CreateSampler(const SamplerDesc& desc) abstract;
		virtual void Release(Sampler& sampler) abstract;

		/* ----- Render Passes ----- */
		virtual RenderPass* CreateRenderPass(const RenderPassDesc& desc) abstract;
		virtual void Release(RenderPass& renderPass) abstract;

		/* ----- Render Targets ----- */
		virtual RenderTarget* CreateRenderTarget(const RenderTargetDesc& desc) abstract;
		virtual void Release(RenderTarget& renderTarget) abstract;

		/* ----- Shader ----- */
		virtual Shader* CreateShader(const ShaderDesc& desc) abstract;
		virtual void Release(Shader& shader) abstract;

		/* ----- Pipeline Layouts ----- */
		virtual PipelineLayout* CreatePipelineLayout(const PipelineLayoutDesc& desc) abstract;
		virtual void Release(PipelineLayout& pipelineLayout) abstract;

		/* ----- Pipeline States ----- */
		virtual PipelineState* CreatePipelineState(const GraphicsPipelineDesc& desc) abstract;
		virtual void Release(PipelineState& pipelineState) abstract;

	};
}