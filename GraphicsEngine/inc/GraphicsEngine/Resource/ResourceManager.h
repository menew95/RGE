#pragma once

#include "Common.h"

#include "GraphicsModule/Core/RenderSystem.h"

namespace Graphics
{
	class RenderSystem;

	class MeshBuffer;
	class MaterialBuffer;

	class ResourceManager
	{
	public:
		ResourceManager(Graphics::RenderSystem* renderSystem);
		~ResourceManager();

		MeshBuffer* CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs);
		MaterialBuffer* CreateMaterialBuffer(uuid uuid, Graphics::PipelineLayout* pipelineLayout);
		
		Shader*				CreateShader(uuid uuid, ShaderDesc& desc);
		Texture*			CreateTexture(uuid uuid, TextureDesc& desc);
		RenderTarget*		CreateRenderTarget(uuid uuid, RenderTargetDesc& desc);
		PipelineState*		CreatePipelineState(uuid uuid, GraphicsPipelineDesc& desc);
		PipelineLayout*		CreatePipelineLayout(uuid uuid, PipelineLayoutDesc& desc);

		RenderPass*			CreateRenderPass(uuid uuid, RenderTargetDesc& desc);

		Shader*				GetShader(uuid uuid);
		Texture*			GetTexture(uuid uuid);
		RenderTarget*		GetRenderTarget(uuid uuid);
		PipelineState*		GetPipelineState(uuid uuid);
		PipelineLayout*		GetPipelineLayout(uuid uuid);

		RenderPass*			GetRenderPass(uuid uuid);

	private:
		Graphics::RenderSystem* m_RenderSystem;

		std::map<uuid, MeshBuffer*> m_MeshBuffers;
		std::map<uuid, MaterialBuffer*> m_MaterialBuffers;

		std::unordered_map<uuid, Shader*> m_ShaderMap;
		std::unordered_map<uuid, Texture*> m_TextureMap;
		std::unordered_map<uuid, RenderTarget*> m_RenderTargetMap;
		std::unordered_map<uuid, PipelineState*> m_PipelineStateMap;
		std::unordered_map<uuid, PipelineLayout*> m_PipelineLayoutMap;
	};
}