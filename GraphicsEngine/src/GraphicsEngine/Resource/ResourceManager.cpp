#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"

#include "GraphicsEngine/RenderPass/RenderPass.h"

#include "Common/StringHelper.h"

namespace Graphics
{

	ResourceManager::ResourceManager(Graphics::RenderSystem* renderSystem)
		: m_RenderSystem(renderSystem)
	{

	}

	ResourceManager::~ResourceManager()
	{
		Release();
	}

	MeshBuffer* ResourceManager::CreateMeshBuffer(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_MeshBufferMap),
			std::end(m_MeshBufferMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_MeshBufferMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a mesh-buffer that already exists.").c_str());
			return nullptr;
		}

		auto* _newMeshBuffer = new MeshBuffer(m_RenderSystem);

		m_MeshBufferMap.insert(std::make_pair(uuid, _newMeshBuffer));

		return _newMeshBuffer;
	}

	Graphics::MeshBuffer* ResourceManager::CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs)
	{
		auto _find = std::find_if(std::begin(m_MeshBufferMap),
			std::end(m_MeshBufferMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_MeshBufferMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a mesh-buffer that already exists.").c_str());
			return nullptr;
		}

		auto* _newMeshBuffer = new MeshBuffer(m_RenderSystem);

		m_MeshBufferMap.insert(std::make_pair(uuid, _newMeshBuffer));

		_newMeshBuffer->CreateVertexBuffer(uuid, vertices);

		for (auto& _subMesh : subMeshs)
		{
			_newMeshBuffer->CreateSubMesh(uuid, _subMesh);
		}

		return _newMeshBuffer;
	}

	Graphics::MaterialBuffer* ResourceManager::CreateMaterialBuffer(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_MaterialBufferMap),
			std::end(m_MaterialBufferMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_MaterialBufferMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a material-buffer that already exists.").c_str());
			return nullptr;
		}

		auto* _newMaterialBuffer = new MaterialBuffer(m_RenderSystem);

		m_MaterialBufferMap.insert(std::make_pair(uuid, _newMaterialBuffer));

		return _newMaterialBuffer;
	}

	Graphics::CameraBuffer* ResourceManager::CreateCameraBuffer()
	{
		static uint32 _idx = 0;
		uuid _uuid = TEXT("Camera") + std::to_wstring(_idx);

		auto _find = std::find_if(std::begin(m_CameraBufferMap),
			std::end(m_CameraBufferMap),
			[&_uuid](auto& pair) { return (_uuid == pair.first); }
		);

		if (_find != m_CameraBufferMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(_uuid) + " is a camera-buffer that already exists.").c_str());
			return nullptr;
		}

		auto* _newCameraBuffer = new CameraBuffer(m_RenderSystem);

		m_CameraBufferMap.insert(std::make_pair(_uuid, _newCameraBuffer));

		return _newCameraBuffer;
	}

	Graphics::MeshBuffer* ResourceManager::GetMeshBuffer(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_MeshBufferMap),
			std::end(m_MeshBufferMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_MeshBufferMap.end())
		{
			return _find->second;
		}

		return nullptr;
	}

	Graphics::MaterialBuffer* ResourceManager::GetMaterialBuffer(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_MaterialBufferMap),
			std::end(m_MaterialBufferMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_MaterialBufferMap.end())
		{
			return _find->second;
		}

		return nullptr;
	}

	Graphics::Buffer* ResourceManager::CreateBuffer(uuid uuid, BufferDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_BufferMap),
			std::end(m_BufferMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_BufferMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a shader that already exists.").c_str());
			return nullptr;
		}

		auto* _newBuffer= m_RenderSystem->CreateBuffer(uuid, desc);

		m_BufferMap.insert(std::make_pair(uuid, _newBuffer));

		return _newBuffer;
	}

	Graphics::Sampler* ResourceManager::CreateSampler(uuid uuid, SamplerDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_SamplerMap),
			std::end(m_SamplerMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_SamplerMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a shader that already exists.").c_str());
			return nullptr;
		}

		auto* _newSampler = m_RenderSystem->CreateSampler(uuid, desc);

		m_SamplerMap.insert(std::make_pair(uuid, _newSampler));

		return _newSampler;
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

	Graphics::RenderPass* ResourceManager::CreateRenderPass(uuid uuid, RenderPassDesc& desc)
	{
		auto _find = std::find_if(std::begin(m_RenderPassMap),
			std::end(m_RenderPassMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_RenderPassMap.end())
		{
			AssertMessageBox(false, (StringHelper::WStringToString(uuid) + " is a render-pass that already exists.").c_str());
			return nullptr;
		}

		auto* _newRenderPass = new RenderPass(desc);

		m_RenderPassMap.insert(std::make_pair(uuid, _newRenderPass));

		return _newRenderPass;
	}

	Graphics::Buffer* ResourceManager::GetBuffer(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_BufferMap),
			std::end(m_BufferMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_BufferMap.end())
		{
			return _find->second;
		}

		assert(false);
		return nullptr;
	}

	Graphics::Sampler* ResourceManager::GetSampler(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_SamplerMap),
			std::end(m_SamplerMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_SamplerMap.end())
		{
			return _find->second;
		}

		assert(false);
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

		assert(false);
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

		//assert(false);
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

		assert(false);
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

		assert(false);
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

		assert(false);
		return nullptr;
	}

	std::shared_ptr<RenderPass> ResourceManager::GetRenderPass(uuid uuid)
	{
		auto _find = std::find_if(std::begin(m_RenderPassMap),
			std::end(m_RenderPassMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_RenderPassMap.end())
		{
			return _find->second;
		}

		assert(false);
		return nullptr;
	}

	Graphics::Texture* ResourceManager::LoadTexture(uuid uuid, ImageDesc* imageDesc)
	{
		auto _find = std::find_if(std::begin(m_TextureMap),
			std::end(m_TextureMap),
			[&uuid](auto& pair) { return (uuid == pair.first); }
		);

		if (_find != m_TextureMap.end())
		{
			return _find->second;
		}

		TextureDesc desc;

		auto* _loadTex = m_RenderSystem->CreateTexture(uuid, desc, imageDesc);

		m_TextureMap.insert(std::make_pair(uuid, _loadTex));

		return _loadTex;
	}

	void ResourceManager::Release()
	{
		for (auto& _meshBuffer : m_MeshBufferMap)
		{
			delete _meshBuffer.second;
		}

		for (auto& _matBuffer : m_MaterialBufferMap)
		{
			delete _matBuffer.second;
		}

		for (auto& _camBuffer : m_CameraBufferMap)
		{
			delete _camBuffer.second;
		}

		for (auto& _buffer : m_BufferMap)
		{
			m_RenderSystem->Release(*_buffer.second);

			_buffer.second = nullptr;
		}

		for (auto& _sampler : m_SamplerMap)
		{
			m_RenderSystem->Release(*_sampler.second);

			_sampler.second = nullptr;
		}

		for (auto& _texture : m_TextureMap)
		{
			m_RenderSystem->Release(*_texture.second);

			_texture.second = nullptr;
		}

		for (auto& _shader : m_ShaderMap)
		{
			m_RenderSystem->Release(*_shader.second);

			_shader.second = nullptr;
		}

		for (auto& _rt : m_RenderTargetMap)
		{
			m_RenderSystem->Release(*_rt.second);

			_rt.second = nullptr;
		}

		for (auto& _state : m_PipelineStateMap)
		{
			m_RenderSystem->Release(*_state.second);

			_state.second = nullptr;
		}

		for (auto& _layout : m_PipelineLayoutMap)
		{
			m_RenderSystem->Release(*_layout.second);

			_layout.second = nullptr;
		}
	}
}