#include "GraphicsEnginePCH.h"

#include "GraphicsModule/Module.h"

#include "GraphicsEngine/GraphicsEngine.h"

#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/ConstantBuffer.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"

#include "GraphicsEngine/Json/JsonTable.h"

HINSTANCE m_GraphicsModule;

namespace Graphics
{
	GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc)
	{
		LoadDllAndCreateRenderSystem();

		m_ResourceManager = std::make_shared<ResourceManager>(m_RenderSystem);

		LoadGraphicsTable();

		Initialize(desc);
	}

	GraphicsEngine::~GraphicsEngine()
	{
		FreeDllAndReleaseRenderSystem();
	}

	void GraphicsEngine::Initialize(const GraphicsEngineDesc& desc)
	{
		SwapChainDesc _swapChainDesc;

		_swapChainDesc._fullScreen = false;
		_swapChainDesc._resolution = { 1200, 720 };
		_swapChainDesc._windowDesc._hwnd = desc._handle;

		m_SwapChain = m_RenderSystem->CreateSwapChain(TEXT("MainSwapChain"), _swapChainDesc);

		CommandBufferDesc _commandBufferDesc;

		m_CommandBuffer = m_RenderSystem->CreateCommandBuffer(TEXT("MainCommandBuffer"), _commandBufferDesc);

		auto* _state = m_ResourceManager->GetPipelineState(TEXT("Deferred_Mesh_Base"));
		auto* _rt = m_ResourceManager->GetRenderTarget(TEXT("Deferred_Mesh"));

		std::vector<AttachmentClear> _attachmentClears =
		{
			{ { 0, 0, 0, 0 }, 0 },
			{ { 0, 0, 0, 0 }, 1 },
			{ { 1, 0, 0, 0 }, 2 },
			{ { 0, 0, 0, 0 }, 3 },
			{ { 0, 0, 0, 0 }, 4 },
			{ 1, 0 }
		};

		m_Deferred_Mesh_Pass = new RenderPass(_state, _rt, _attachmentClears);

		m_Deferred_Mesh_Pass->SetPerFrameBuffer(m_ResourceManager->GetBuffer(TEXT("cbPerFrame")));
		m_Deferred_Mesh_Pass->SetPerObjectBuffer(m_ResourceManager->GetBuffer(TEXT("cbPerObject")));

		_state = m_ResourceManager->GetPipelineState(TEXT("Deferred_Merge"));
		//_rt = m_ResourceManager->GetRenderTarget(TEXT("Deferred_Light"));

		_attachmentClears =
		{
			{ { 1, 0, 0, 0 }, 0 },
			{ 1, 0 }
		};

		m_Deferred_Light_Pass = new RenderPass(_state, m_SwapChain, _attachmentClears);

		m_Deferred_Light_Pass->SetPerFrameBuffer(m_ResourceManager->GetBuffer(TEXT("cbPerLightFrame")));
		m_Deferred_Light_Pass->SetPerObjectBuffer(m_ResourceManager->GetBuffer(TEXT("cbPerObject")));

		IntiLightPass();

		//m_Final_Pass = new RenderPass(_state, m_SwapChain);
	}

	void GraphicsEngine::IntiLightPass()
	{
		m_Deferred_Light_Material = m_ResourceManager->CreateMaterialBuffer(TEXT("Light"), m_ResourceManager->GetPipelineLayout(TEXT("Deferred_Light_Layout")));

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
	}

	Graphics::MeshBuffer* GraphicsEngine::CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs)
	{
		return m_ResourceManager->CreateMeshBuffer(uuid, vertices, subMeshs);
	}

	Graphics::MaterialBuffer* GraphicsEngine::CreateMaterialBuffer(uuid uuid, PipelineLayout* pipelineLayout)
	{
		return m_ResourceManager->CreateMaterialBuffer(uuid, pipelineLayout);
	}

	Graphics::MaterialBuffer* GraphicsEngine::CreateMaterialBuffer(uuid uuid, const tstring& pipelineLayout)
	{
		auto _pipelineLayout = m_ResourceManager->GetPipelineLayout(pipelineLayout);
		auto _newMatBuffer = m_ResourceManager->CreateMaterialBuffer(uuid, _pipelineLayout);

		_newMatBuffer->SetRenderPass(m_Deferred_Mesh_Pass);

		return _newMatBuffer;
	}

	Graphics::CameraBuffer* GraphicsEngine::CreateCameraBuffer()
	{
		m_MainCameraBuffer = m_ResourceManager->CreateCameraBuffer();

		return m_MainCameraBuffer;
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
		{
			m_CommandBuffer->BeginEvent(TEXT("Mesh"));

			// Update Frame Buffer
			PerFrame _perFrame;

			m_MainCameraBuffer->UpdateCamera(_perFrame._camera);

			m_Deferred_Mesh_Pass->UpdatePerFrame(m_CommandBuffer, &_perFrame, sizeof(_perFrame));

			m_Deferred_Mesh_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Pass->EndExcute(m_CommandBuffer);

			m_CommandBuffer->EndEvent();
		}

		{
			RenderObject _deferredMergeRenderObject;
			_deferredMergeRenderObject.m_MeshBuffer = m_Screen_Mesh;
			_deferredMergeRenderObject.m_MaterialBuffer = m_Deferred_Light_Material;

			m_Deferred_Light_Pass->RegistRenderObject(_deferredMergeRenderObject);


			m_CommandBuffer->BeginEvent(TEXT("Light"));
			
			// Update Frame Buffer
			PerLightFrame _perLightFrame;
			_perLightFrame._lightCount = 1;
			_perLightFrame._iblFactor = 0;

			//  Todo : 게임 엔진으로 부터 라이트들을 받아오도록 만들어야 함
			
			//_perLightFrame._perLights[0] = ;


			m_Deferred_Light_Pass->UpdatePerFrame(m_CommandBuffer, &_perLightFrame, sizeof(_perLightFrame));

			m_Deferred_Light_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Light_Pass->Excute(m_CommandBuffer);

			m_Deferred_Light_Pass->EndExcute(m_CommandBuffer);

			m_CommandBuffer->ResetResourceSlots(ResourceType::Texture, 0, 5, BindFlags::ShaderResource, StageFlags::PS);

			m_CommandBuffer->EndEvent();
		}

		/*{
			RenderObject _deferredMergeRenderObject;
			_deferredMergeRenderObject.m_MeshBuffer = m_Screen_Mesh;
			_deferredMergeRenderObject.m_MaterialBuffer = m_Deferred_Light_Material;

			m_Final_Pass->BeginEvent(TEXT("Final"));

			m_Final_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Final_Pass->Excute(m_CommandBuffer);

			m_Final_Pass->EndExcute(m_CommandBuffer);

			m_CommandBuffer->ResetResourceSlots(ResourceType::Texture, 0, 5, BindFlags::ShaderResource, StageFlags::PS);

			m_CommandBuffer->EndEvent();
		}*/

		m_SwapChain->Present();
	}

	void GraphicsEngine::LoadGraphicsTable()
	{
		TableLoader::LoadShaderTable(m_ResourceManager.get());
		TableLoader::LoadBufferTable(m_ResourceManager.get());
		TableLoader::LoadSamplerTable(m_ResourceManager.get());
		TableLoader::LoadTextureTable(m_ResourceManager.get());
		TableLoader::LoadRenderTargetTable(m_ResourceManager.get(), { 1280.f, 720.f });
		TableLoader::LoadRenderingPipelineTable(m_ResourceManager.get());
		TableLoader::LoadPipelineStateTable(m_ResourceManager.get());
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