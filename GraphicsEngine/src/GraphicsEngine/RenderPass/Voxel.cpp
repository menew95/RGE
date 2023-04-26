#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/RenderPass/Voxel.h"

#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/ConstantBuffer.h"

namespace Graphics
{
	DECLSPEC_ALIGN(16) struct VoxelCBuffer
	{
		DirectX::XMFLOAT3 grid_center;
		float   data_size;        // voxel half-extent in world space units
		float   data_size_rcp;    // 1.0 / voxel-half extent
		uint32    data_res;         // voxel grid resolution
		float   data_res_rcp;     // 1.0 / voxel grid resolution
		uint32    num_cones;
		float   num_cones_rcp;
		float   max_distance;
		float   ray_step_size;
		uint32    mips;
	};

	Voxel::Voxel(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
	{
		Initialize();
	}

	Voxel::~Voxel()
	{

	}

	void Voxel::Excute()
	{
		Viewport _viewport{ 0, 0, static_cast<float>(VOXEL_RESOLUTION), static_cast<float>(VOXEL_RESOLUTION), 0.f, 1.f };

		m_CommandBuffer->BeginEvent(TEXT("Voxelization Pass"));

		m_CommandBuffer->SetViewport(_viewport);

		m_CommandBuffer->SetPipelineState(*m_VoxelizePSO);

		for (auto& _renderObject : m_RenderObjectList)
		{

			auto _vertexBuffer = _renderObject->GetMeshBuffer()->GetBuffer();

			m_CommandBuffer->SetVertexBuffer(*_vertexBuffer);


			for (uint32 _subMeshCnt = 0; _subMeshCnt < _renderObject->GetMeshBuffer()->GetSubMeshCount(); _subMeshCnt++)
			{
				auto _subMeshBuffer = _renderObject->GetMeshBuffer()->GetSubMesh(_subMeshCnt);

				m_CommandBuffer->SetIndexBuffer(*_subMeshBuffer.m_IndexBuffer);

				m_CommandBuffer->SetResources(*m_VoxelizeLayout);

				m_CommandBuffer->DrawIndexed(_subMeshBuffer.m_IndexCount, 0, 0);
			}
		}

		m_CommandBuffer->ResetResourceSlots(ResourceType::Buffer, 0, 1, BindFlags::UnorderedAccess, StageFlags::PS);

		m_CommandBuffer->EndEvent();
	}

	static constexpr uint32 GetCBufferSize(uint32 buffer_size)
	{
		return (buffer_size + (64 - 1)) & ~(64 - 1);
	}

	void Voxel::CreateVoxelResource()
	{
		BufferDesc _bufferDesc{};
		
		_bufferDesc._stride = sizeof(VoxelType);
		_bufferDesc._size = _bufferDesc._stride * VOXEL_RESOLUTION * VOXEL_RESOLUTION * VOXEL_RESOLUTION;
		_bufferDesc._bindFlags = BindFlags::ShaderResource | BindFlags::UnorderedAccess;
		_bufferDesc._miscFlags = MiscFlags::DynamicUsage;

		m_Voxel = m_ResourceManager->CreateBuffer(TEXT("VoxelBuffer"), _bufferDesc);

		BufferDesc _voxelDataDesc{};

		_voxelDataDesc._size = GetCBufferSize(sizeof(VoxelCBuffer));
		_voxelDataDesc._stride = 0;
		_voxelDataDesc._bindFlags = BindFlags::ConstantBuffer;
		_voxelDataDesc._slot = 4;

		m_VoxelData = m_ResourceManager->CreateBuffer(TEXT("VoxelData"), _voxelDataDesc);

		TextureDesc _textureDesc;

		_textureDesc._textureType = TextureType::Texture3D;
		_textureDesc._extend = { VOXEL_RESOLUTION, VOXEL_RESOLUTION , VOXEL_RESOLUTION };
		_textureDesc._miscFlags = MiscFlags::GenerateMips;
		_textureDesc._mipLevels = 1;
		_textureDesc._bindFlags = BindFlags::UnorderedAccess | BindFlags::ShaderResource | BindFlags::RenderTarget;
		_textureDesc._format = Format::R16G16B16A16_FLOAT;

		m_VoxelTexture = m_ResourceManager->CreateTexture(TEXT("VoxelTexture"), _textureDesc);
		m_VoxelBoundTexture = m_ResourceManager->CreateTexture(TEXT("VoxelBoundTexture"), _textureDesc);
	}

	void Voxel::CreateVoxelizePass()
	{
		ShaderDesc _voxelizeVS;
		{
			_voxelizeVS._shaderType = ShaderType::Vertex;
			_voxelizeVS._sourceType = ShaderSourceType::HLSL;
			_voxelizeVS._filePath = TEXT("Asset\\Shader\\VS_Voxelization.hlsl");
			_voxelizeVS._sourceSize = 0;
			_voxelizeVS._entryPoint = "main";
			_voxelizeVS._profile = "vs_5_0";
		}

		ShaderDesc _voxelizeGS;
		{
			_voxelizeVS._shaderType = ShaderType::Geometry;
			_voxelizeVS._sourceType = ShaderSourceType::HLSL;
			_voxelizeVS._filePath = TEXT("Asset\\Shader\\GS_Voxelization.hlsl");
			_voxelizeVS._sourceSize = 0;
			_voxelizeVS._entryPoint = "main";
			_voxelizeVS._profile = "gs_5_0";
		}

		ShaderDesc _voxelizePS;
		{
			_voxelizeVS._shaderType = ShaderType::Pixel;
			_voxelizeVS._sourceType = ShaderSourceType::HLSL;
			_voxelizeVS._filePath = TEXT("Asset\\Shader\\PS_Voxelization.hlsl");
			_voxelizeVS._sourceSize = 0;
			_voxelizeVS._entryPoint = "main";
			_voxelizeVS._profile = "ps_5_0";
		}

		PipelineLayoutDesc _voxelizeLayoutDesc;
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::VS | StageFlags::GS | StageFlags::PS, 1
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::GS | StageFlags::PS, 4
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::UnorderedAccess, StageFlags::PS, 0
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_Voxel);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::PS, 0
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
		}
		{
			BindingDescriptor _bindDesc;
			{
				ResourceType::Sampler, 0, StageFlags::PS, 0;
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_VoxelData);
		}

		m_VoxelizeLayout = m_ResourceManager->CreatePipelineLayout(TEXT("Voxelize"), _voxelizeLayoutDesc);

		GraphicsPipelineDesc _voxelizePSODesc;

		_voxelizePSODesc._pipelineLayout = m_VoxelizeLayout;

		_voxelizePSODesc._shaderProgram._vertexShader = m_ResourceManager->CreateShader(TEXT("VS_Voxelization"), _voxelizeVS);
		_voxelizePSODesc._shaderProgram._geometryShader = m_ResourceManager->CreateShader(TEXT("GS_Voxelization"), _voxelizeGS);
		_voxelizePSODesc._shaderProgram._pixelShader = m_ResourceManager->CreateShader(TEXT("PS_Voxelization"), _voxelizePS);

		_voxelizePSODesc._hasDSS = false;
		_voxelizePSODesc._hasBS = false;

		_voxelizePSODesc._rasterizerDesc._cullMode = CullMode::None;
		_voxelizePSODesc._rasterizerDesc._fillMode = FillMode::Solid;
		_voxelizePSODesc._rasterizerDesc._depthClampEnabled = true;
		_voxelizePSODesc._rasterizerDesc._multiSampleEnabled = true;

		m_VoxelizePSO = m_ResourceManager->CreatePipelineState(TEXT("Voxelize"), _voxelizePSODesc);
	}

	void Voxel::CreateVoxelCopyPass()
	{
		ShaderDesc _voxelCopyDesc;
		{
			_voxelCopyDesc._shaderType = ShaderType::Compute;
			_voxelCopyDesc._sourceType = ShaderSourceType::HLSL;
			_voxelCopyDesc._filePath = TEXT("Asset\\Shader\\CS_VoxelCopy.hlsl");
			_voxelCopyDesc._sourceSize = 0;
			_voxelCopyDesc._entryPoint = "main";
			_voxelCopyDesc._profile = "cs_5_0";
		}

		auto* _copy = m_ResourceManager->CreateShader(TEXT("VoxelCopy"), _voxelCopyDesc);

		PipelineLayoutDesc _voxelCopyLayoutDesc;
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::CS, 4
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::UnorderedAccess, StageFlags::CS, 0
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::UnorderedAccess, StageFlags::CS, 1
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
		}

		m_VoxelCopyLayout = m_ResourceManager->CreatePipelineLayout(TEXT("VoxelCopy"), _voxelCopyLayoutDesc);

		ComputePipelineDesc _voxelCSODesc;
		_voxelCSODesc.pipelineLayout = m_VoxelCopyLayout;
		_voxelCSODesc._shaderProgram._computeShader = _copy;

		m_VoxelCopyCSO = m_ResourceManager->CreatePipelineState(TEXT("VoxelCopy"), _voxelCSODesc);
	}

	void Voxel::UpdateResourcePerMaterial(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout)
	{
		auto& _sources = renderObject->GetUpdateResourceData();

		for (size_t i = 0; i < _sources.size(); i++)
		{
			assert(pipelineLayout != nullptr);

			switch (_sources[i]._resourceType)
			{
				case ResourceType::Buffer:
				{
					auto _buffer = pipelineLayout->GetBuffer(_sources[i]._index);

					UpdateBuffer(commandBuffer, _buffer, _sources[i]._dataSrc, _sources[i]._datasize);
					break;
				}
				case ResourceType::Sampler:
				case ResourceType::Texture:
				{
					pipelineLayout->SetResource(_sources[i]._index, reinterpret_cast<Resource*>(_sources[i]._dataSrc));
					break;
				}
				case ResourceType::Undefined:
				default:
				{
					assert(false);
					break;
				}
			}
		}
	}

	void Voxel::UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout)
	{
		auto& _sources = renderObject->GetUpdateResourceDataPerObject();

		for (size_t i = 0; i < _sources.size(); i++)
		{
			assert(pipelineLayout != nullptr);

			switch (_sources[i]._resourceType)
			{
				case ResourceType::Buffer:
				{
					auto _buffer = pipelineLayout->GetBuffer(_sources[i]._index);

					UpdateBuffer(commandBuffer, _buffer, _sources[i]._dataSrc, _sources[i]._datasize);
					break;
				}
				case ResourceType::Sampler:
				case ResourceType::Texture:
				{
					pipelineLayout->SetResource(_sources[i]._index, reinterpret_cast<Resource*>(_sources[i]._dataSrc));
					break;
				}
				case ResourceType::Undefined:
				default:
				{
					assert(false);
					break;
				}
			}
		}
	}

	void Voxel::UpdateBuffer(CommandBuffer* commandBuffer, Buffer* buffer, void* src, uint32 size)
	{
		commandBuffer->UpdateBuffer(*buffer, 0, src, size);
	}

	void Voxel::RegistRenderObject(RenderObject* renderObject)
	{
		m_RenderObjectList.push_back(renderObject);
	}

	void Voxel::Initialize()
	{
		CreateVoxelResource();

		CreateVoxelizePass();

		CreateVoxelCopyPass();
	}

}