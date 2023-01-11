#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/GraphicsEngine.h"

#include "GraphicsModule/Module.h"

HINSTANCE m_GraphicsModule;

namespace Graphics
{


	GraphicsEngine::GraphicsEngine()
	{
		LoadDllAndCreateRenderSystem();
	}

	GraphicsEngine::~GraphicsEngine()
	{
		FreeDllAndReleaseRenderSystem();
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

}