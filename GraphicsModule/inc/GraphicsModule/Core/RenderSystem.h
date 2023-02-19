#pragma once

#include "GraphicsModule/Utility/Helper.h"

#include "GraphicsModule/Core/Buffer.h"
#include "GraphicsModule/Core/Texture.h"
#include "GraphicsModule/Core/Sampler.h"
#include "GraphicsModule/Core/CommandBuffer.h"
#include "GraphicsModule/Core/ResourceFlags.h"
#include "GraphicsModule/Core/RenderSystemFlags.h"

#include "GraphicsModule/Core/Shader.h"
#include "GraphicsModule/Core/SwapChain.h"
//#include "GraphicsModule/Core/RenderPass.h"
#include "GraphicsModule/Core/RenderTarget.h"
#include "GraphicsModule/Core/PipelineLayout.h"
#include "GraphicsModule/Core/PipelineState.h"

#include "GraphicsModule/Core/VideoAdapter.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC RenderSystem
	{
	public:
		RenderSystem() = default;
		virtual ~RenderSystem() = default;

		/*static std::vector<std::string> FindModules();
		static std::unique_ptr<RenderSystem> Load(RenderSystemDesc& desc);
		static void Unload(std::unique_ptr<RenderSystem>&& renderSystem);*/

		/* ----- SwapChain ----- */
		virtual SwapChain* CreateSwapChain(uuid uuid, const SwapChainDesc& desc) abstract;
		virtual void Release(SwapChain& swapChain) abstract;

		/* ----- Command Buffer ----- */
		virtual CommandBuffer* CreateCommandBuffer(uuid uuid, const CommandBufferDesc& desc) abstract;
		virtual void Release(CommandBuffer& commandBuffer) abstract;

		/* ----- Buffer ----- */
		virtual Buffer* CreateBuffer(uuid uuid, const BufferDesc& desc, const void* initData = nullptr) abstract;
		virtual void Release(Buffer& buffer) abstract;

		virtual void WriteBuffer(Buffer& buffer, const void* data, uint32 dataSize, uint32 offset) abstract;
		virtual void ReadBuffer(Buffer& buffer, void* data, uint32 dataSize, uint32 offset) abstract;

		virtual void* MapBuffer(Buffer& buffer, const CPUAccess access) abstract;
		virtual void* MapBuffer(Buffer& buffer, const CPUAccess access, uint32 offset, uint32 length) abstract;
		virtual void UnmapBuffer(Buffer& buffer) abstract;

		/* ----- Textures ----- */
		virtual Texture* CreateTexture(uuid uuid, const TextureDesc& desc, const struct ImageDesc* imageDesc = nullptr) abstract;
		virtual void Release(Texture& texture) abstract;

		virtual void WriteTexture(Texture& texture, const TextureRegion& textureRegion, const ImageDesc& imageDesc) abstract;
		virtual void ReadTexture(Texture& texture) abstract;

		/* ----- Samplers ---- */
		virtual Sampler* CreateSampler(uuid uuid, const SamplerDesc& desc) abstract;
		virtual void Release(Sampler& sampler) abstract;

		///* ----- Render Passes ----- */
		//virtual RenderPass* CreateRenderPass(uuid uuid, const RenderPassDesc& desc) abstract;
		//virtual void Release(RenderPass& renderPass) abstract;

		/* ----- Render Targets ----- */
		virtual RenderTarget* CreateRenderTarget(uuid uuid, const RenderTargetDesc& desc) abstract;
		virtual void Release(RenderTarget& renderTarget) abstract;

		/* ----- Shader ----- */
		virtual Shader* CreateShader(uuid uuid, const ShaderDesc& desc) abstract;
		virtual void Release(Shader& shader) abstract;

		/* ----- Pipeline Layouts ----- */
		virtual PipelineLayout* CreatePipelineLayout(uuid uuid, const PipelineLayoutDesc& desc) abstract;
		virtual void Release(PipelineLayout& pipelineLayout) abstract;

		/* ----- Pipeline States ----- */
		virtual PipelineState* CreatePipelineState(uuid uuid, const GraphicsPipelineDesc& desc) abstract;
		virtual void Release(PipelineState& pipelineState) abstract;
	};
}