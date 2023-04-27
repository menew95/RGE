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

#include "GraphicsEngine/RenderQueue/RenderQueue.h"

#include "GraphicsEngine/RenderPass/IBL.h"
#include "GraphicsEngine/RenderPass/Light.h"
#include "GraphicsEngine/RenderPass/Deferred.h"
#include "GraphicsEngine/RenderPass/Sky.h"
#include "GraphicsEngine/RenderPass/DebugDeferred.h"
#include "GraphicsEngine/RenderPass/PostProcess.h"
#include "GraphicsEngine/RenderPass/Voxel.h"

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
		m_CommandBuffer->EndRenderPass();
		m_CommandBuffer->ClearState();

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

		InitSkyPass();

		InitDebugPass();

		InitIBL();

		InitCascadedShadow();

		InitLight();

		InitSSR();

		InitVoxel();
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

		m_Deferred_Mesh_Albedo_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Albedo Pass"));

		m_Deferred_Mesh_Albedo_Bump_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Albedo_Bump Pass"));

		m_Deferred_Mesh_Albedo_Bump_MRA_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Albedo_Bump_MRA Pass"));

		m_Deferred_Mesh_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin Pass"));

		m_Deferred_Mesh_Skinned_Albedo_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Albedo Pass"));

		m_Deferred_Mesh_Skinned_Albedo_Bump_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Albedo_Bump Pass"));

		m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Albedo_Bump_MRA Pass"));

		m_Deferred = std::make_shared<Deferred>(m_CommandBuffer, m_ResourceManager);
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
			{ { -1.f, -1.f, 1.0f }, { 0, 1 } },
			{ { -1.f, +1.f, 1.0f }, { 0, 0 } },
			{ { +1.f, -1.f, 1.0f }, { 1, 1 } },
			{ { +1.f, +1.f, 1.0f }, { 1, 0 } }
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

	void GraphicsEngine::InitSkyPass()
	{
		m_Sky = std::make_shared<Sky>(m_CommandBuffer, m_ResourceManager);

		m_Sky->SetRenderTarget(m_SwapChain);
	}

	void GraphicsEngine::InitDebugPass()
	{
		m_Debug_Deferred = std::make_shared<DebugDeferred>(m_CommandBuffer, m_ResourceManager);

		m_Debug_Deferred->SetSwapChain(m_SwapChain);
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

	void GraphicsEngine::InitLight()
	{
		m_Light = std::make_shared<Light>(m_RenderSystem, m_CommandBuffer, m_ResourceManager);

	}

	void GraphicsEngine::InitCascadedShadow()
	{
		m_CascadedShadow_Pass = m_ResourceManager->GetRenderPass(TEXT("CascadedShadow Pass"));
		m_CascadedShadow_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("CascadedShadow_Skinned Pass"));

		m_PointShadow_Pass = m_ResourceManager->GetRenderPass(TEXT("PointLightShadow Pass"));
		m_PointShadow_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("PointLightShadow_Skinned Pass"));
	}

	void GraphicsEngine::InitSSR()
	{
		m_SSR_Pass = std::make_shared<PostProcess>(m_CommandBuffer, m_ResourceManager);
	}

	void GraphicsEngine::InitVoxel()
	{
		m_Voxel_Pass = std::make_shared<Voxel>(m_CommandBuffer, m_ResourceManager);
	}

	Graphics::MeshBuffer* GraphicsEngine::CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs, Math::Vector3 min, Math::Vector3 max)
	{
		return m_ResourceManager->CreateMeshBuffer(uuid, vertices, subMeshs, min, max);
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
		return m_Light->AddLight();
	}

	Graphics::Texture* GraphicsEngine::LoadTexture(uuid uuid, ImageDesc* imageDesc)
	{
		return m_ResourceManager->LoadTexture(uuid, imageDesc);
	}

	void GraphicsEngine::OnResize(uint32 _width, uint32 _height)
	{
		m_SwapChain->ResizeBuffer({ _width, _height });
	}

	void GraphicsEngine::RegistRenderObject(RenderObject* renderObject)
	{
		assert(renderObject != nullptr);

		m_RenderObjectList.emplace_back(renderObject);
	}

	void GraphicsEngine::DeleteRenderObject(RenderObject* renderObject)
	{
		auto _iter = std::find(m_RenderObjectList.begin(), m_RenderObjectList.end(), renderObject);

		assert(_iter != m_RenderObjectList.end());

		m_RenderObjectList.erase(_iter);
	}

	void GraphicsEngine::RegistRenderMesh(RenderObject& renderObject)
	{
		m_Deferred->RegistRenderObject(renderObject);

		m_Voxel_Pass->RegistRenderObject(renderObject);
	}

	void GraphicsEngine::RegistRenderShadow(uint32 type, RenderObject& renderObject)
	{
		switch (type)
		{
			case 0:
			{
				m_Light->RegistStaticRenderObject(renderObject);
				break;
			}
			case 1:
			{
				m_Light->RegistSkinnedRenderObject(renderObject);
				break;
			}
			default:
				assert("Attempt to register undefined type in shadow object list", false);
				break;
		}
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
		Lighting* _perLighting = m_Light->GetLightingData();

		UpdateResourceData _perDraw{ eUpdateTime::PerObject, 1, ResourceType::Buffer, _perLighting, sizeof(Lighting) };
		_deferredMergeRenderObject.m_UpdateResourcePerObjects.push_back(_perDraw);

		m_MainCameraBuffer->UpdateCascadeShadow(_perLighting->_directionLight[0]._direction, _perLighting->_cascadedLight);

		m_CascadedShadow_Pass->UpdatePerFrame(m_CommandBuffer, &_perLighting->_cascadedLight, sizeof(_perLighting->_cascadedLight));

		m_Light->ExcutePass();
		
		m_Deferred->SetCameraBuffer(m_MainCameraBuffer);

		m_Deferred->ExcutePass();

		m_SSR_Pass->ExcutePass();

		m_Sky->ExcutePass();


		{
			m_Deferred_Light_Pass->RegistRenderObject(&_deferredMergeRenderObject);

			m_Deferred_Light_Pass->UpdatePerFrame(m_CommandBuffer, _perLighting, sizeof(Lighting));

			m_Deferred_Light_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Light_Pass->Excute(m_CommandBuffer);

			m_Deferred_Light_Pass->EndExcute(m_CommandBuffer);
		}

		m_Debug_Deferred->ExcutePass();


		m_Voxel_Pass->UpdateVoxelInfo(_perFrame._camera._camWorld, 4, 2, 0.75f, 20.0f);

		m_Voxel_Pass->Excute();

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