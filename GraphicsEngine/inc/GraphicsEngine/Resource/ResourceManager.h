#pragma once

#include "Common.h"

#include "GraphicsModule/Core/RenderSystem.h"

namespace Graphics
{
	struct RenderPassDesc;

	class RenderSystem;

	class MeshBuffer;
	class MaterialBuffer;
	class CameraBuffer;

	class ResourceManager
	{
	public:
		ResourceManager(Graphics::RenderSystem* renderSystem);
		~ResourceManager();

		MeshBuffer*						CreateMeshBuffer(uuid uuid);
		MeshBuffer*						CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs);
		MaterialBuffer*					CreateMaterialBuffer(uuid uuid, Graphics::PipelineLayout* pipelineLayout);
		CameraBuffer*					CreateCameraBuffer();

		MeshBuffer*						GetMeshBuffer(uuid uuid);
		MaterialBuffer*					GetMaterialBuffer(uuid uuid);

		Buffer*							CreateBuffer(uuid uuid, BufferDesc& desc);
		Sampler*						CreateSampler(uuid uuid, SamplerDesc& desc);
		Texture*						CreateTexture(uuid uuid, TextureDesc& desc);

		Shader*							CreateShader(uuid uuid, ShaderDesc& desc);
		RenderTarget*					CreateRenderTarget(uuid uuid, RenderTargetDesc& desc);
		PipelineState*					CreatePipelineState(uuid uuid, GraphicsPipelineDesc& desc);
		PipelineLayout*					CreatePipelineLayout(uuid uuid, PipelineLayoutDesc& desc);

		RenderPass*						CreateRenderPass(uuid uuid, RenderPassDesc& desc);

		Buffer*							GetBuffer(uuid uuid);
		Sampler*						GetSampler(uuid uuid);
		Texture*						GetTexture(uuid uuid);

		Shader*							GetShader(uuid uuid);
		RenderTarget*					GetRenderTarget(uuid uuid);
		PipelineState*					GetPipelineState(uuid uuid);
		PipelineLayout*					GetPipelineLayout(uuid uuid);

		std::shared_ptr<RenderPass>		GetRenderPass(uuid uuid);
		std::unordered_map<uuid, std::shared_ptr<RenderPass>>		GetRenderPasss() { return m_RenderPassMap; }


		Texture* LoadTexture(uuid uuid, ImageDesc* imageDesc);

	private:
		void Release();

		Graphics::RenderSystem* m_RenderSystem;

		std::map<uuid, MeshBuffer*> m_MeshBufferMap;
		std::map<uuid, MaterialBuffer*> m_MaterialBufferMap;
		std::map<uuid, CameraBuffer*> m_CameraBufferMap;

		// Resource
		std::unordered_map<uuid, Buffer*> m_BufferMap;
		std::unordered_map<uuid, Sampler*> m_SamplerMap;
		std::unordered_map<uuid, Texture*> m_TextureMap;

		std::unordered_map<uuid, Shader*> m_ShaderMap;
		std::unordered_map<uuid, RenderTarget*> m_RenderTargetMap;
		std::unordered_map<uuid, PipelineState*> m_PipelineStateMap;
		std::unordered_map<uuid, PipelineLayout*> m_PipelineLayoutMap;

		std::unordered_map<uuid, std::shared_ptr<RenderPass>> m_RenderPassMap;
	};
}