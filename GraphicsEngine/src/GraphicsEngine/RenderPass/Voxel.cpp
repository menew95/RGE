#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/RenderPass/Voxel.h"

#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/ConstantBuffer.h"

namespace Graphics
{
	static constexpr uint32 GetCBufferSize(uint32 buffer_size)
	{
		return (buffer_size + (64 - 1)) & ~(64 - 1);
	}

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
		ExcuteVoxelize();

		ExcuteCopy();

		ExcuteDebug();
	}

	void Voxel::ExcuteVoxelize()
	{
		Viewport _viewport{ 0, 0, static_cast<float>(VOXEL_RESOLUTION), static_cast<float>(VOXEL_RESOLUTION), 0.f, 1.f };

		m_CommandBuffer->BeginEvent(TEXT("Voxelization Pass"));

		m_CommandBuffer->SetViewport(_viewport);

		m_CommandBuffer->SetPipelineState(*m_VoxelizePSO1);

		for (auto& _renderObject : m_RenderObjectList)
		{
			auto _vertexBuffer = _renderObject.GetMeshBuffer()->GetBuffer();

			m_CommandBuffer->SetVertexBuffer(*_vertexBuffer);

			UpdateResourcePerObject(m_CommandBuffer, &_renderObject, m_VoxelizeLayout);

			for (uint32 _subMeshCnt = 0; _subMeshCnt < _renderObject.GetMeshBuffer()->GetSubMeshCount(); _subMeshCnt++)
			{
				auto _subMeshBuffer = _renderObject.GetMeshBuffer()->GetSubMesh(_subMeshCnt);

				// Albedo
				auto& _sources = _renderObject.GetUpdateResourceData();

				m_VoxelizeLayout->SetResource(3, reinterpret_cast<Resource*>(_sources[0]._dataSrc));

				m_CommandBuffer->SetIndexBuffer(*_subMeshBuffer.m_IndexBuffer);

				m_CommandBuffer->SetResources(*m_VoxelizeLayout);

				m_CommandBuffer->DrawIndexed(_subMeshBuffer.m_IndexCount, 0, 0);
			}
		}

		m_CommandBuffer->ResetResourceSlots(ResourceType::Buffer, 0, 1, BindFlags::UnorderedAccess, StageFlags::PS);
		
		m_CommandBuffer->EndEvent();

		m_RenderObjectList.clear();
	}

	void Voxel::ExcuteCopy()
	{
		m_CommandBuffer->BeginEvent(TEXT("Voxel Copy Pass"));

		m_CommandBuffer->SetPipelineState(*m_VoxelCopyCSO);

		m_CommandBuffer->SetResource(*m_VoxelData, 5, BindFlags::ConstantBuffer, StageFlags::CS);
		m_CommandBuffer->SetResource(*m_Voxel, 0, BindFlags::UnorderedAccess, StageFlags::CS);
		m_CommandBuffer->SetResource(*m_VoxelTexture, 1, BindFlags::UnorderedAccess, StageFlags::CS);

		m_CommandBuffer->Dispatch(VOXEL_RESOLUTION * VOXEL_RESOLUTION * VOXEL_RESOLUTION / 256, 1, 1);

		m_CommandBuffer->ResetResourceSlots(ResourceType::Texture, 0, 2, BindFlags::UnorderedAccess, StageFlags::CS);

		//m_CommandBuffer->GenerateMips(*m_VoxelTexture);

		m_CommandBuffer->EndEvent();
	}

	void Voxel::ExcuteDebug()
	{
		m_CommandBuffer->BeginEvent(TEXT("Voxel Debug Pass"));

		m_CommandBuffer->SetViewport({ 0, 0, 1280, 720, 0, 1 });

		m_CommandBuffer->SetPipelineState(*m_VoxelDebugLinePSO);

		m_CommandBuffer->SetRenderTarget(*m_RenderTarget, 0, 0);
		
		m_CommandBuffer->SetResources(*m_VoxelDebugLayout);

		m_CommandBuffer->Draw(VOXEL_RESOLUTION * VOXEL_RESOLUTION * VOXEL_RESOLUTION, 0);

		m_CommandBuffer->ResetResourceSlots(ResourceType::Texture, 0, 1, BindFlags::ShaderResource, StageFlags::VS);

		m_CommandBuffer->EndEvent();
	}

	void Voxel::UpdateVoxelInfo(Vector3 camPos, float voxelSize, float coneNum, float rayStepDis, float maxDis)
	{
		float const f = 0.05f / voxelSize;

		Vector3 center = Vector3(floorf(camPos.x * f) / f, floorf(camPos.y * f) / f, floorf(camPos.z * f) / f);

		m_Voxel_Info.data_res = VOXEL_RESOLUTION;
		m_Voxel_Info.data_res_rcp = 1.0f / VOXEL_RESOLUTION;
		m_Voxel_Info.data_size = voxelSize;
		m_Voxel_Info.data_size_rcp = 1.0f / voxelSize;
		m_Voxel_Info.mips = 7;
		m_Voxel_Info.num_cones = static_cast<uint32>(coneNum);
		m_Voxel_Info.num_cones_rcp = 1.0f / coneNum;
		m_Voxel_Info.ray_step_size = rayStepDis;
		m_Voxel_Info.max_distance = maxDis;
		m_Voxel_Info.grid_center = center;
		
		m_CommandBuffer->UpdateBuffer(*m_VoxelData, 0, &m_Voxel_Info, GetCBufferSize(sizeof(VoxelInfoCB)));
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

		_voxelDataDesc._size = GetCBufferSize(sizeof(VoxelInfoCB));
		_voxelDataDesc._stride = 0;
		_voxelDataDesc._bindFlags = BindFlags::ConstantBuffer;
		_voxelDataDesc._slot = 4;

		m_VoxelData = m_ResourceManager->CreateBuffer(TEXT("VoxelData"), _voxelDataDesc);

		UpdateVoxelInfo({ 0, 0, 0 }, 4, 2, 0.75f, 20.0f);

		TextureDesc _textureDesc;

		_textureDesc._textureType = TextureType::Texture3D;
		_textureDesc._extend = { VOXEL_RESOLUTION, VOXEL_RESOLUTION , VOXEL_RESOLUTION };
		_textureDesc._miscFlags = MiscFlags::GenerateMips;
		_textureDesc._mipLevels = 7;
		_textureDesc._bindFlags = BindFlags::UnorderedAccess | BindFlags::ShaderResource | BindFlags::RenderTarget;
		_textureDesc._format = Format::R16G16B16A16_FLOAT;

		m_VoxelTexture = m_ResourceManager->CreateTexture(TEXT("VoxelTexture"), _textureDesc);
		//m_VoxelBoundTexture = m_ResourceManager->CreateTexture(TEXT("VoxelBoundTexture"), _textureDesc);

		//RenderTargetDesc _renderTargetDesc;

		//_renderTargetDesc._extend = { 1280, 720 };
		//_renderTargetDesc._sample = 1;

		//{
		//	AttachmentDesc _attachDesc;
		//	_attachDesc._renderTargetType = RenderTargetType::RenderTarget;
		//	_attachDesc._resource = m_ResourceManager->GetTexture(TEXT("Degerred_Light"));
		//	_attachDesc._mipLevel = 0;
		//	_renderTargetDesc._attachments.push_back(_attachDesc);
		//}

		//{
		//	AttachmentDesc _attachDesc;
		//	_attachDesc._renderTargetType = RenderTargetType::DepthStencil;
		//	_attachDesc._resource = m_ResourceManager->GetTexture(TEXT("DeferredDSV"));
		//	_attachDesc._mipLevel = 0;
		//	_renderTargetDesc._attachments.push_back(_attachDesc);
		//}

		//m_RenderTarget = m_ResourceManager->CreateRenderTarget(TEXT("VoxelDebugRT"), _renderTargetDesc);
	}

	void Voxel::CreateVoxelizePass()
	{
		ShaderMacro _albedoMap{ "_ALBEDO_MAP", "" };
		ShaderMacro _normalMap{ "_NORMAL_MAP", "" };
		ShaderMacro _mraMap{ "_MRA_MAP", "" };
		ShaderMacro _null{ NULL, NULL };

		std::vector<ShaderMacro> _macro{ _null, _null, _null, _null };

#pragma region NONE TEXTURE
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
			_voxelizeGS._shaderType = ShaderType::Geometry;
			_voxelizeGS._sourceType = ShaderSourceType::HLSL;
			_voxelizeGS._filePath = TEXT("Asset\\Shader\\GS_Voxelization.hlsl");
			_voxelizeGS._sourceSize = 0;
			_voxelizeGS._entryPoint = "main";
			_voxelizeGS._profile = "gs_5_0";
		}

		ShaderDesc _voxelizePS;
		{
			_voxelizePS._shaderType = ShaderType::Pixel;
			_voxelizePS._sourceType = ShaderSourceType::HLSL;
			_voxelizePS._filePath = TEXT("Asset\\Shader\\PS_Voxelization.hlsl");
			_voxelizePS._sourceSize = 0;
			_voxelizePS._entryPoint = "main";
			_voxelizePS._profile = "ps_5_0";
		}

		PipelineLayoutDesc _voxelizeLayoutDesc;
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::GS | StageFlags::PS, 5
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_VoxelData);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::VS | StageFlags::GS | StageFlags::PS, 1
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetBuffer(TEXT("Transform")));
		}
		/*{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::VS | StageFlags::GS | StageFlags::PS, 4
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetBuffer(TEXT("Lighting")));
		}*/
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
			_voxelizeLayoutDesc._resources.push_back(nullptr);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::PS, 5
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetTexture(TEXT("Pre_Filtered")));
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::PS, 6
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetTexture(TEXT("Irradiance")));
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::PS, 7
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetTexture(TEXT("IntegrateBRDF")));
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::PS, 8
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetTexture(TEXT("CascadedShadow")));
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::PS, 9
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetTexture(TEXT("SpotLightShadow")));
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::PS, 10
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetTexture(TEXT("PointLightShadow")));
		}
		{
			BindingDescriptor _bindDesc;
			{
				ResourceType::Sampler, 0, StageFlags::PS, 0;
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetSampler(TEXT("samWrapLinear")));
		}
		{
			BindingDescriptor _bindDesc;
			{
				ResourceType::Sampler, 0, StageFlags::PS, 1;
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetSampler(TEXT("samCascaded")));
		}
		{
			BindingDescriptor _bindDesc;
			{
				ResourceType::Sampler, 0, StageFlags::PS, 2;
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetSampler(TEXT("samPointLight")));
		}
		{
			BindingDescriptor _bindDesc;
			{
				ResourceType::Sampler, 0, StageFlags::PS, 3;
			};

			_voxelizeLayoutDesc._bindings.push_back(_bindDesc);
			_voxelizeLayoutDesc._resources.push_back(m_ResourceManager->GetSampler(TEXT("samPoint")));
		}

		m_VoxelizeLayout = m_ResourceManager->CreatePipelineLayout(TEXT("Voxelize"), _voxelizeLayoutDesc);

		GraphicsPipelineDesc _voxelizePSODesc;

		_voxelizePSODesc._pipelineLayout = m_VoxelizeLayout;

		_voxelizePSODesc._shaderProgram._vertexShader = m_ResourceManager->CreateShader(TEXT("VS_Voxelization"), _voxelizeVS);
		_voxelizePSODesc._shaderProgram._geometryShader = m_ResourceManager->CreateShader(TEXT("GS_Voxelization"), _voxelizeGS);
		_voxelizePSODesc._shaderProgram._pixelShader = m_ResourceManager->CreateShader(TEXT("PS_Voxelization"), _voxelizePS);

		_voxelizePSODesc._hasBS = false;

		_voxelizePSODesc._rasterizerDesc._cullMode = CullMode::None;
		_voxelizePSODesc._rasterizerDesc._fillMode = FillMode::Solid;
		_voxelizePSODesc._rasterizerDesc._depthClampEnabled = false;
		_voxelizePSODesc._rasterizerDesc._multiSampleEnabled = true;

		_voxelizePSODesc._depthDesc._depthEnabled = false;
		_voxelizePSODesc._stencilDesc._stencilEnable = false;

		m_VoxelizePSO = m_ResourceManager->CreatePipelineState(TEXT("Voxelize"), _voxelizePSODesc);
		
#pragma endregion NONE TEXTURE

#pragma region ALBEDO

		//_macro.push_back(_albedoMap);
		_macro[0] = _albedoMap;

		_voxelizeVS._defines = _macro.data();

		_voxelizeGS._defines = _macro.data();
		
		_voxelizePS._defines = _macro.data();

		m_VoxelizeLayout1 = m_ResourceManager->CreatePipelineLayout(TEXT("Voxelize1"), _voxelizeLayoutDesc);

		_voxelizePSODesc._pipelineLayout = m_VoxelizeLayout1;

		_voxelizePSODesc._shaderProgram._vertexShader = m_ResourceManager->CreateShader(TEXT("VS_Voxelization1"), _voxelizeVS);
		_voxelizePSODesc._shaderProgram._geometryShader = m_ResourceManager->CreateShader(TEXT("GS_Voxelization1"), _voxelizeGS);
		_voxelizePSODesc._shaderProgram._pixelShader = m_ResourceManager->CreateShader(TEXT("PS_Voxelization1"), _voxelizePS);

		m_VoxelizePSO1 = m_ResourceManager->CreatePipelineState(TEXT("Voxelize1"), _voxelizePSODesc);
		
#pragma endregion ALBEDO

#pragma region ALBEDOMAP + NORMAL

		_macro[1] = _normalMap;
		
		m_VoxelizeLayout2 = m_ResourceManager->CreatePipelineLayout(TEXT("Voxelize2"), _voxelizeLayoutDesc);

		_voxelizePSODesc._pipelineLayout = m_VoxelizeLayout;

		_voxelizePSODesc._shaderProgram._vertexShader = m_ResourceManager->CreateShader(TEXT("VS_Voxelization2"), _voxelizeVS);
		_voxelizePSODesc._shaderProgram._geometryShader = m_ResourceManager->CreateShader(TEXT("GS_Voxelization2"), _voxelizeGS);
		_voxelizePSODesc._shaderProgram._pixelShader = m_ResourceManager->CreateShader(TEXT("PS_Voxelization2"), _voxelizePS);

		m_VoxelizePSO = m_ResourceManager->CreatePipelineState(TEXT("Voxelize2"), _voxelizePSODesc);

#pragma endregion ALBEDO + NORMAL

#pragma region ALBEDO + NORMAL + MRA

		_macro[2] = _mraMap;

		m_VoxelizeLayout3 = m_ResourceManager->CreatePipelineLayout(TEXT("Voxelize3"), _voxelizeLayoutDesc);

		_voxelizePSODesc._pipelineLayout = m_VoxelizeLayout;

		_voxelizePSODesc._shaderProgram._vertexShader = m_ResourceManager->CreateShader(TEXT("VS_Voxelization3"), _voxelizeVS);
		_voxelizePSODesc._shaderProgram._geometryShader = m_ResourceManager->CreateShader(TEXT("GS_Voxelization3"), _voxelizeGS);
		_voxelizePSODesc._shaderProgram._pixelShader = m_ResourceManager->CreateShader(TEXT("PS_Voxelization3"), _voxelizePS);

		m_VoxelizePSO3 = m_ResourceManager->CreatePipelineState(TEXT("Voxelize3"), _voxelizePSODesc);
		
#pragma endregion ALBEDO + NORMAL + MRA
		
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
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::CS, 5
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
			_voxelCopyLayoutDesc._resources.push_back(m_VoxelData);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::UnorderedAccess, StageFlags::CS, 0
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
			_voxelCopyLayoutDesc._resources.push_back(m_Voxel);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::UnorderedAccess, StageFlags::CS, 1
			};

			_voxelCopyLayoutDesc._bindings.push_back(_bindDesc);
			_voxelCopyLayoutDesc._resources.push_back(m_VoxelTexture);
		}

		m_VoxelCopyLayout = m_ResourceManager->CreatePipelineLayout(TEXT("VoxelCopy"), _voxelCopyLayoutDesc);

		ComputePipelineDesc _voxelCopyCSODesc;
		_voxelCopyCSODesc.pipelineLayout = m_VoxelCopyLayout;
		_voxelCopyCSODesc._shaderProgram._computeShader = _copy;

		m_VoxelCopyCSO = m_ResourceManager->CreatePipelineState(TEXT("VoxelCopy"), _voxelCopyCSODesc);
	}

	void Voxel::CreateVoxelDebugPass()
	{
		ShaderDesc _voxelDebugVS;
		{
			_voxelDebugVS._shaderType = ShaderType::Vertex;
			_voxelDebugVS._sourceType = ShaderSourceType::HLSL;
			_voxelDebugVS._filePath = TEXT("Asset\\Shader\\VS_VoxelDebug.hlsl");
			_voxelDebugVS._sourceSize = 0;
			_voxelDebugVS._entryPoint = "main";
			_voxelDebugVS._profile = "vs_5_0";
		}

		ShaderDesc _voxelDebugGS;
		{
			_voxelDebugGS._shaderType = ShaderType::Geometry;
			_voxelDebugGS._sourceType = ShaderSourceType::HLSL;
			_voxelDebugGS._filePath = TEXT("Asset\\Shader\\GS_VoxelDebug.hlsl");
			_voxelDebugGS._sourceSize = 0;
			_voxelDebugGS._entryPoint = "main";
			_voxelDebugGS._profile = "gs_5_0";
		}

		ShaderDesc _voxelDebugLineGS;
		{
			_voxelDebugLineGS._shaderType = ShaderType::Geometry;
			_voxelDebugLineGS._sourceType = ShaderSourceType::HLSL;
			_voxelDebugLineGS._filePath = TEXT("Asset\\Shader\\GS_VoxelDebugLine.hlsl");
			_voxelDebugLineGS._sourceSize = 0;
			_voxelDebugLineGS._entryPoint = "main";
			_voxelDebugLineGS._profile = "gs_5_0";
		}

		ShaderDesc _voxelDebugPS;
		{
			_voxelDebugPS._shaderType = ShaderType::Pixel;
			_voxelDebugPS._sourceType = ShaderSourceType::HLSL;
			_voxelDebugPS._filePath = TEXT("Asset\\Shader\\PS_VoxelDebug.hlsl");
			_voxelDebugPS._sourceSize = 0;
			_voxelDebugPS._entryPoint = "main";
			_voxelDebugPS._profile = "ps_5_0";
		}

		PipelineLayoutDesc _pipelineDesc;
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Texture, BindFlags::ShaderResource, StageFlags::VS, 0
			};

			_pipelineDesc._bindings.push_back(_bindDesc);
			_pipelineDesc._resources.push_back(m_VoxelTexture);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::VS | StageFlags::GS, 5
			};

			_pipelineDesc._bindings.push_back(_bindDesc);
			_pipelineDesc._resources.push_back(m_VoxelData);
		}
		{
			BindingDescriptor _bindDesc
			{
				ResourceType::Buffer, BindFlags::ConstantBuffer, StageFlags::GS, 0
			};

			_pipelineDesc._bindings.push_back(_bindDesc);
			_pipelineDesc._resources.push_back(m_ResourceManager->GetBuffer(TEXT("PerCamera")));
		}

		m_VoxelDebugLayout = m_ResourceManager->CreatePipelineLayout(TEXT("VoxelDebug"), _pipelineDesc);

		GraphicsPipelineDesc _voxelDebugPSODesc;

		_voxelDebugPSODesc._pipelineLayout = m_VoxelDebugLayout;

		_voxelDebugPSODesc._shaderProgram._vertexShader = m_ResourceManager->CreateShader(TEXT("VS_VoxelDebug"), _voxelDebugVS);
		_voxelDebugPSODesc._shaderProgram._geometryShader = m_ResourceManager->CreateShader(TEXT("GS_VoxelDebug"), _voxelDebugGS);
		_voxelDebugPSODesc._shaderProgram._pixelShader = m_ResourceManager->CreateShader(TEXT("PS_VoxelDebug"), _voxelDebugPS);

		_voxelDebugPSODesc._primitiveTopology = PrimitiveTopology::PointList;

		_voxelDebugPSODesc._viewports.push_back({ 0, 0, 1280, 720, 0, 1 });

		_voxelDebugPSODesc._depthDesc._depthEnabled = true;
		_voxelDebugPSODesc._depthDesc.compareOp = CompareOp::Less;
		_voxelDebugPSODesc._depthDesc._writeEnabled = false;

		_voxelDebugPSODesc._stencilDesc._stencilEnable = false;
		_voxelDebugPSODesc._stencilDesc._readMask = 0;
		_voxelDebugPSODesc._stencilDesc._writeMask = 0;
		_voxelDebugPSODesc._stencilDesc._back = { StencilOp::Keep, StencilOp::Keep , StencilOp::Replace, CompareOp::Equal };
		_voxelDebugPSODesc._stencilDesc._front = { StencilOp::Keep, StencilOp::Keep , StencilOp::Replace, CompareOp::Equal };

		_voxelDebugPSODesc._rasterizerDesc._cullMode = CullMode::Back;
		_voxelDebugPSODesc._rasterizerDesc._fillMode = FillMode::Solid;
		_voxelDebugPSODesc._rasterizerDesc._frontCCW = false;
		_voxelDebugPSODesc._rasterizerDesc._depthBias.clamp = 0;
		_voxelDebugPSODesc._rasterizerDesc._depthBias.slopeFactor = 1;
		_voxelDebugPSODesc._rasterizerDesc._depthBias.constantFactor = 10000;

		_voxelDebugPSODesc._hasBS = false;
		
		m_VoxelDebugPSO = m_ResourceManager->CreatePipelineState(TEXT("VoxelDebug"), _voxelDebugPSODesc);

		// Line Debug

		//_voxelDebugPSODesc._rasterizerDesc._fillMode = FillMode::WireFrame;

		_voxelDebugPSODesc._shaderProgram._geometryShader = m_ResourceManager->CreateShader(TEXT("GS_VoxelDebugLine"), _voxelDebugLineGS);

		m_VoxelDebugLinePSO = m_ResourceManager->CreatePipelineState(TEXT("VoxelDebugLine"), _voxelDebugPSODesc);

		m_RenderTarget = m_ResourceManager->GetRenderTarget(TEXT("FinalRT"));
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
	void Voxel::RegistRenderObject(RenderObject& renderObject)
	{
		auto& _sources = renderObject.GetUpdateResourceData();

		if (_sources.size() == 0) return;

		m_RenderObjectList.push_back(renderObject);
	}

	void Voxel::Initialize()
	{
		CreateVoxelResource();

		CreateVoxelizePass();

		CreateVoxelCopyPass();

		CreateVoxelDebugPass();
	}

}