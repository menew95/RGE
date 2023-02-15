#include "GraphicsEnginePCH.h"

#include "GraphicsModule/Module.h"

#include "GraphicsEngine/GraphicsEngine.h"

#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/ConstantBuffer.h"
#include "GraphicsEngine/Resource/LightBuffer.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"

#include "GraphicsEngine/Json/JsonTable.h"

#include "GraphicsEngine/RenderPass/IBL.h"

HINSTANCE m_GraphicsModule;

namespace Graphics
{
	GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc)
	{
		LoadDllAndCreateRenderSystem();

		m_ResourceManager = new ResourceManager(m_RenderSystem);

		LoadGraphicsTable();

		Initialize(desc);
	}

	GraphicsEngine::~GraphicsEngine()
	{
		m_CommandBuffer->ResetResourceSlots(ResourceType::Texture, 0, 8, StageFlags::AllStages);
		m_CommandBuffer->EndRenderPass();

		m_RenderSystem->Release(*m_SwapChain);
		m_RenderSystem->Release(*m_CommandBuffer);

		delete m_ResourceManager;

		FreeDllAndReleaseRenderSystem();
	}

	void GraphicsEngine::Initialize(const GraphicsEngineDesc& desc)
	{
		SwapChainDesc _swapChainDesc;

		_swapChainDesc._fullScreen = false;
		_swapChainDesc._resolution = { 1280, 720 };
		_swapChainDesc._windowDesc._hwnd = desc._handle;

		m_SwapChain = m_RenderSystem->CreateSwapChain(TEXT("MainSwapChain"), _swapChainDesc);

		CommandBufferDesc _commandBufferDesc;

		m_CommandBuffer = m_RenderSystem->CreateCommandBuffer(TEXT("MainCommandBuffer"), _commandBufferDesc);

		InitMeshPass();

		InitLightPass();

		InitSkyBoxPass();

		InitDebugPass();

		InitIBL();
	}

	void GraphicsEngine::InitMeshPass()
	{
		std::vector<AttachmentClear> _attachmentClears =
		{
			{ { 0, 0, 0, 0 }, 0 },
			{ { 0, 0, 0, 0 }, 1 },
			{ { 1, 0, 0, 0 }, 2 },
			{ { 0, 0, 0, 0 }, 3 },
			{ { 0, 0, 0, 0 }, 4 },
			{ 1, 0 }
		};

		m_Deferred_Mesh_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh Pass"));
		m_Deferred_Mesh_Pass->SetAttachmentClears(_attachmentClears);

		m_Deferred_Mesh_Bump_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Bump Pass"));

		m_Deferred_Mesh_Bump_MRA_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Bump_MRA Pass"));

		m_Deferred_Mesh_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin Pass"));


		m_Deferred_Mesh_Skinned_Bump_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Bump Pass"));

		m_Deferred_Mesh_Skinned_Bump_MRA_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Bump_MRA Pass"));
	}

	void GraphicsEngine::InitLightPass()
	{
		m_Deferred_Light_Material = m_ResourceManager->CreateMaterialBuffer(TEXT("Light"));

		m_Screen_Mesh = m_ResourceManager->CreateMeshBuffer(TEXT("Screen_Mesh"));

		struct ScreenVertex
		{
			Math::Vector3 pos;
			Math::Vector2 uv;
		};

		std::vector<ScreenVertex> _data =
		{
			{ { -1.f, -1.f, 0.0f }, { 0, 1 } },
			{ { -1.f, +1.f, 0.0f }, { 0, 0 } },
			{ { +1.f, -1.f, 0.0f }, { 1, 1 } },
			{ { +1.f, +1.f, 0.0f }, { 1, 0 } }
		};

		std::vector<uint32> triangles =
		{
			0,
			3,
			2,
			0,
			1,
			3
		};

		uint32 _size = static_cast<uint32>(sizeof(ScreenVertex) * _data.size());

		m_Screen_Mesh->CreateVertexBuffer(TEXT("Screen_Mesh"), _data.data(), _size, sizeof(ScreenVertex));
		m_Screen_Mesh->CreateSubMesh(TEXT("Screen_Mesh"), triangles);

		m_Deferred_Light_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Light Pass"));

		m_Deferred_Light_Pass->SetRenderTarget(m_SwapChain);
	}

	void GraphicsEngine::InitSkyBoxPass()
	{
		m_SkyBox_Material = m_ResourceManager->CreateMaterialBuffer(TEXT("SkyBox"));

		m_SkyBox_Mesh = m_ResourceManager->CreateMeshBuffer(TEXT("Cube_Mesh"));

		std::vector<Math::Vector3> _data =
		{
			{ -1.0f, +1.0f, -1.0f },
			{ +1.0f, +1.0f, -1.0f },
			{ +1.0f, +1.0f, +1.0f },
			{ -1.0f, +1.0f, +1.0f },
			{ -1.0f, -1.0f, -1.0f },
			{ +1.0f, -1.0f, -1.0f },
			{ +1.0f, -1.0f, +1.0f },
			{ -1.0f, -1.0f, +1.0f }
		};

		std::vector<uint32> triangles =
		{
			// +x
			1, 5, 2,
			2, 5, 6,
			// -x
			0, 3, 7,
			0, 7, 4,
			// +y
			0, 2, 3,
			0, 1, 2,
			// -y
			4, 6, 5,
			4, 7, 6,
			// +z
			2, 7, 3,
			2, 6, 7,
			// -z
			0, 4, 1,
			1, 4, 5
		};

		uint32 _size = static_cast<uint32>(sizeof(Math::Vector3) * _data.size());

		m_SkyBox_Mesh->CreateVertexBuffer(TEXT("Cube_Mesh"), _data.data(), _size, sizeof(Math::Vector3));
		m_SkyBox_Mesh->CreateSubMesh(TEXT("Cube_Mesh"), triangles);

		auto* _state = m_ResourceManager->GetPipelineState(TEXT("SkyBox"));
		auto* _layout = m_ResourceManager->GetPipelineLayout(TEXT("SkyBox_Layout"));

		std::vector<AttachmentClear> _attachmentClears =
		{
			{ { 1, 0, 0, 0 }, 0 },
			{ 1, 0 }
		};

		m_SkyBox_Pass = m_ResourceManager->GetRenderPass(TEXT("Skybox Pass"));
		m_SkyBox_Pass->SetRenderTarget(m_SwapChain);
		//m_SkyBox_Pass->SetAttachmentClears(_attachmentClears);

		RenderObject _skyBoxObject;
		_skyBoxObject.m_MeshBuffer = m_SkyBox_Mesh;
		_skyBoxObject.m_MaterialBuffer = m_SkyBox_Material;

		m_SkyBox_Pass->RegistRenderObject(_skyBoxObject);
	}

	void GraphicsEngine::InitDebugPass()
	{
		m_Debug_Material = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug"));

		m_Albedo = m_ResourceManager->GetTexture(TEXT("Albedo"));
		m_Normal = m_ResourceManager->GetTexture(TEXT("Normal"));
		m_Depth = m_ResourceManager->GetTexture(TEXT("Depth"));
		m_World = m_ResourceManager->GetTexture(TEXT("WorldPosition"));

		RenderObject _albedoObject;
		_albedoObject.m_MeshBuffer = m_Screen_Mesh;
		_albedoObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceA{ 1,  m_Albedo };
		_albedoObject.m_UpdateResources.push_back(_resourceA);

		RenderObject _normalObject;
		_normalObject.m_MeshBuffer = m_Screen_Mesh;
		_normalObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceN{ 1,  m_Normal };
		_normalObject.m_UpdateResources.push_back(_resourceN);

		RenderObject _depthObject;
		_depthObject.m_MeshBuffer = m_Screen_Mesh;
		_depthObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceD{ 1,  m_Depth };
		_depthObject.m_UpdateResources.push_back(_resourceD);

		RenderObject _worldPosObject;
		_worldPosObject.m_MeshBuffer = m_Screen_Mesh;
		_worldPosObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceW{ 1,  m_World };
		_worldPosObject.m_UpdateResources.push_back(_resourceW);

		m_DebugRenderObject.push_back(_albedoObject);
		m_DebugRenderObject.push_back(_normalObject);
		m_DebugRenderObject.push_back(_depthObject);
		m_DebugRenderObject.push_back(_worldPosObject);

		m_Debug_Pass = m_ResourceManager->GetRenderPass(TEXT("MRT Debug Pass"));

		m_Debug_Pass->SetRenderTarget(m_SwapChain);
		m_Debug_Pass->AddResourceClear({ ResourceType::Texture, 0, 1, BindFlags::ShaderResource, StageFlags::PS });

		for (uint i = 0; i < m_DebugRenderObject.size(); i++)
		{
			Math::Viewport _viewport{ 256.f * i, 0.f, 256.f, 144.f };
			//Math::Viewport _viewport{ 128.f, 0.f, 128.f, 72.f  };

			m_DebugRenderObject[i].AddViewport(_viewport);

			m_Debug_Pass->RegistRenderObject(m_DebugRenderObject[i]);
		}
	}

	void GraphicsEngine::InitIBL()
	{
		m_IBL = std::make_shared<IBL>(m_CommandBuffer, m_ResourceManager);

		CreateIBL();
	}

	void GraphicsEngine::CreateIBL()
	{
		// 나중엔 텍스처(로드한 스카이 박스 혹은 동적으로 캡처된 큐브맵)를 변수로 받아 생성
		m_IBL->CreateIBLResource();
	}

	Graphics::MeshBuffer* GraphicsEngine::CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs)
	{
		return m_ResourceManager->CreateMeshBuffer(uuid, vertices, subMeshs);
	}

	Graphics::MaterialBuffer* GraphicsEngine::CreateMaterialBuffer(uuid uuid)
	{
		return m_ResourceManager->CreateMaterialBuffer(uuid);
	}

	Graphics::CameraBuffer* GraphicsEngine::CreateCameraBuffer()
	{
		m_MainCameraBuffer = m_ResourceManager->CreateCameraBuffer();

		return m_MainCameraBuffer;
	}

	Graphics::LightBuffer* GraphicsEngine::CreateLightBuffer()
	{
		m_LightBuffers.push_back(std::make_shared<LightBuffer>(m_RenderSystem));

		return m_LightBuffers.back().get();
	}

	Graphics::Texture* GraphicsEngine::LoadTexture(uuid uuid, ImageDesc* imageDesc)
	{
		return m_ResourceManager->LoadTexture(uuid, imageDesc);
	}

	void GraphicsEngine::OnResize(uint32 _width, uint32 _height)
	{
		m_SwapChain->ResizeBuffer({ _width, _height });
	}

	void GraphicsEngine::RegistRenderObject(RenderObject& renderObject)
	{
		m_Deferred_Mesh_Pass->RegistRenderObject(renderObject);
	}

	void GraphicsEngine::Excute()
	{
		PerFrame _perFrame;

		m_MainCameraBuffer->UpdateCamera(_perFrame._camera);

		m_Deferred_Mesh_Pass->UpdatePerFrame(m_CommandBuffer, &_perFrame, sizeof(_perFrame));


		RenderObject _deferredMergeRenderObject;
		_deferredMergeRenderObject.m_MeshBuffer = m_Screen_Mesh;
		_deferredMergeRenderObject.m_MaterialBuffer = m_Deferred_Light_Material;

		// Update Per Object Buffer(Lighting)
		Lighting _perLighting;
		GetLightingData(_perLighting);

		UpdateResourceData _perDraw{ eUpdateTime::PerObject, 1, ResourceType::Buffer, &_perLighting, sizeof(Lighting) };

		_deferredMergeRenderObject.m_UpdateResourcePerObjects.push_back(_perDraw);

		m_Deferred_Light_Pass->RegistRenderObject(_deferredMergeRenderObject);

		{
			m_Deferred_Mesh_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Bump_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Bump_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Bump_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Bump_MRA_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Bump_MRA_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Bump_MRA_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Skinned_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Skinned_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Skinned_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Skinned_Bump_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Skinned_Bump_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Skinned_Bump_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Skinned_Bump_MRA_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Skinned_Bump_MRA_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Skinned_Bump_MRA_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_SkyBox_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_SkyBox_Pass->Excute(m_CommandBuffer);

			m_SkyBox_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Light_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Light_Pass->Excute(m_CommandBuffer);

			m_Deferred_Light_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Debug_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Debug_Pass->Excute(m_CommandBuffer);

			m_Debug_Pass->EndExcute(m_CommandBuffer);
		}

		m_SwapChain->Present();
	}

	void GraphicsEngine::ExcuteRenderPass(Graphics::RenderPass* renderPass)
	{
		renderPass->BeginExcute(m_CommandBuffer, nullptr);

		renderPass->Excute(m_CommandBuffer);

		renderPass->EndExcute(m_CommandBuffer);
	}

	void GraphicsEngine::Present()
	{
		
	}

	Graphics::RenderPass* GraphicsEngine::GetRenderPass(uuid uuid)
	{
		auto* _renderPass = m_ResourceManager->GetRenderPass(uuid).get();

		return _renderPass;
	}

	void GraphicsEngine::GetLightingData(Lighting& perLightFrame)
	{
		perLightFrame._lightCount = static_cast<uint32>(m_LightBuffers.size());

		for (uint32 i = 0; i < m_LightBuffers.size(); i++)
		{
			if (m_LightBuffers[i]->GetEnable())
			{
				perLightFrame._perLights[i] = m_LightBuffers[i]->GetPerLight();
			}
		}
	}

	void GraphicsEngine::LoadGraphicsTable()
	{
		TableLoader::LoadShaderTable(m_ResourceManager);
		TableLoader::LoadBufferTable(m_ResourceManager);
		TableLoader::LoadSamplerTable(m_ResourceManager);
		TableLoader::LoadTextureTable(m_ResourceManager);
		TableLoader::LoadRenderTargetTable(m_ResourceManager, { 1280.f, 720.f });
		TableLoader::LoadRenderingPipelineTable(m_ResourceManager);
		TableLoader::LoadPipelineStateTable(m_ResourceManager);
		TableLoader::LoadRenderPassTable(m_ResourceManager);
	}

	void GraphicsEngine::LoadDllAndCreateRenderSystem()
	{
		std::string _moduleName = "GraphicsModule";

#ifdef _DEBUG
#ifdef x64
		_moduleName += "_Debug_x64.dll";
#else
		_moduleName += "_Debug_x86.dll";
#endif // x64
#else
#ifdef x64
		_moduleName += "_Release_x64.dll";
#else
		_moduleName += "_Release_x64.dll";
#endif // x64
#endif

		m_GraphicsModule = LoadLibraryA(_moduleName.c_str());

		assert(m_GraphicsModule != NULL);

		using CreateRenderSystem = RenderSystem * (*)(RenderSystemDesc&);

		auto createRenderSystem = (CreateRenderSystem)LoadProcedure("CreateRenderSystem");

		RenderSystemDesc _desc;
		_desc._systemType = SystemType::DirectX11;

		m_RenderSystem = createRenderSystem(_desc);
	}

	void GraphicsEngine::FreeDllAndReleaseRenderSystem()
	{
		using ReleaseModule = void (*)(RenderSystem*);

		auto releaseRenderSystem = (ReleaseModule)LoadProcedure("ReleaseRenderSystem");

		releaseRenderSystem(m_RenderSystem);
	}

	void* GraphicsEngine::LoadProcedure(const char* procedureName)
	{
		auto procAddr = GetProcAddress(m_GraphicsModule, procedureName);
		return reinterpret_cast<void*>(procAddr);
	}

	GRAPHICSENGINE_DLL_DECLSPEC GraphicsEngine* CreateGraphicsEngine(GraphicsEngineDesc& desc)
	{
		return new GraphicsEngine(desc);
	}

	GRAPHICSENGINE_DLL_DECLSPEC void ReleaseGraphicsEngine(GraphicsEngine* graphicsEngine)
	{
		delete graphicsEngine;
	}

}