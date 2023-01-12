#include "GraphicsEnginePCH.h"

#include "GraphicsModule/Module.h"

#include "GraphicsEngine/GraphicsEngine.h"

#include "GraphicsEngine/Resource/ResourceManager.h"


HINSTANCE m_GraphicsModule;

namespace Graphics
{
	GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc)
	{
		LoadDllAndCreateRenderSystem();

		m_ResourceManager = std::make_shared<ResourceManager>(m_RenderSystem);
	}

	GraphicsEngine::~GraphicsEngine()
	{
		FreeDllAndReleaseRenderSystem();
	}

	Graphics::MeshBuffer* GraphicsEngine::CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs)
	{
		return m_ResourceManager->CreateMeshBuffer(uuid, vertices, subMeshs);
	}

	Graphics::MaterialBuffer* GraphicsEngine::CreateMaterialBuffer(uuid uuid, PipelineLayout* pipelineLayout)
	{
		return m_ResourceManager->CreateMaterialBuffer(uuid, pipelineLayout);
	}

	void GraphicsEngine::OnResize(uint32 _width, uint32 _height)
	{
		m_SwapChain->ResizeBuffer({ _width, _height });
	}

	void GraphicsEngine::RegistRenderObject(RenderObject& renderObject)
	{

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