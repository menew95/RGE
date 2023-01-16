#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"

#include "Common/StringHelper.h"

namespace Graphics
{

	ResourceManager::ResourceManager(Graphics::RenderSystem* renderSystem)
		: m_RenderSystem(renderSystem)
	{

	}

	ResourceManager::~ResourceManager()
	{

	}

	

	Graphics::MeshBuffer* ResourceManager::CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs)
	{
		//auto iter = std::find_if(std::begin(m_MeshBuffers), std::end(m_MeshBuffers),
		//		[&uuid](auto& _pair) = > {
		//		return (_pair.first == uuid)
		//	}
		//);

		return nullptr;
	}

	Graphics::MaterialBuffer* ResourceManager::CreateMaterialBuffer(uuid uuid, Graphics::PipelineLayout* pipelineLayout)
	{
		return nullptr;
	}

	Shader* ResourceManager::CreateShader(uuid uuid, ShaderDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_ShaderMap),
			std::end(m_ShaderMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_ShaderMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a shader that already exists.").c_str());
			return nullptr;
		}

		auto* _newShader = m_RenderSystem->CreateShader(uuid, desc);

		m_ShaderMap.insert(std::make_pair(uuid, _newShader));

		return _newShader;
	}

	Texture* ResourceManager::CreateTexture(uuid uuid, TextureDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_TextureMap),
			std::end(m_TextureMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_TextureMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a texture that already exists.").c_str());
			return nullptr;
		}

		auto* _newTex = m_RenderSystem->CreateTexture(uuid, desc);

		m_TextureMap.insert(std::make_pair(uuid, _newTex));

		return _newTex;
	}

	RenderTarget* ResourceManager::CreateRenderTarget(uuid uuid, RenderTargetDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_RenderTargetMap),
			std::end(m_RenderTargetMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_RenderTargetMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a render-target that already exists.").c_str());
			return nullptr;
		}

		auto* _newRT = m_RenderSystem->CreateRenderTarget(uuid, desc);

		m_RenderTargetMap.insert(std::make_pair(uuid, _newRT));

		return _newRT;
	}

	PipelineState* ResourceManager::CreatePipelineState(uuid uuid, GraphicsPipelineDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_PipelineStateMap),
			std::end(m_PipelineStateMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_PipelineStateMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a pipeline-state that already exists.").c_str());
			return nullptr;
		}

		auto* _newState = m_RenderSystem->CreatePipelineState(uuid, desc);

		m_PipelineStateMap.insert(std::make_pair(uuid, _newState));

		return _newState;
	}

	PipelineLayout* ResourceManager::CreatePipelineLayout(uuid uuid, PipelineLayoutDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_PipelineLayoutMap),
			std::end(m_PipelineLayoutMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_PipelineLayoutMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a pipeline-layout that already exists.").c_str());
			return nullptr;
		}

		auto* _newLayout = m_RenderSystem->CreatePipelineLayout(uuid, desc);

		m_PipelineLayoutMap.insert(std::make_pair(uuid, _newLayout));

		return _newLayout;
	}

	RenderPass* ResourceManager::CreateRenderPass(uuid uuid, RenderTargetDesc& desc)
	{
		/*auto _find = std::find_if(std::begin(m_RenderPassMap),
			std::end(m_RenderPassMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_RenderPassMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a render-pass that already exists.").c_str());
			return nullptr;
		}

		auto* _newRenderPass = m_RenderSystem->CreatePipelineLayout(uuid, desc);

		m_RenderPassMap.insert(std::make_pair(uuid, _newRenderPass));

		return _newRenderPass;*/

		return nullptr;
	}

	Shader* ResourceManager::GetShader(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_ShaderMap),
			std::end(m_ShaderMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_ShaderMap.end())
		{
			return _find->second;
		}

		return nullptr;
	}

	Texture* ResourceManager::GetTexture(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_TextureMap),
			std::end(m_TextureMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_TextureMap.end())
		{
			return _find->second;
		}
			
		return nullptr;
	}

	RenderTarget* ResourceManager::GetRenderTarget(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_RenderTargetMap),
			std::end(m_RenderTargetMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_RenderTargetMap.end())
		{
			return _find->second;
		}

		return nullptr;
	}

	PipelineState* ResourceManager::GetPipelineState(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_PipelineStateMap),
			std::end(m_PipelineStateMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_PipelineStateMap.end())
		{
			return _find->second;
		}

		return nullptr;
	}
	
	PipelineLayout* ResourceManager::GetPipelineLayout(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_PipelineLayoutMap),
			std::end(m_PipelineLayoutMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_PipelineLayoutMap.end())
		{
			return _find->second;
		}

		return nullptr;
	}
	RenderPass* ResourceManager::GetRenderPass(uuid uuid)
	{
		return nullptr;
	}
}