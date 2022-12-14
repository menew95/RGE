#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GraphicsEngine/GraphicsEngine.h"

#include "GameEngine/Window.h"

HMODULE g_GraphicsEngineModule;

namespace GameEngine
{
	namespace Core
	{
		void* LoadProcedure(const char* procedureName)
		{
			auto procAddr = GetProcAddress(g_GraphicsEngineModule, procedureName);
			return reinterpret_cast<void*>(procAddr);
		}

		DEFINE_SINGLETON_CLASS(GraphicsSystem, {},
			{
				FreeGraphicsEngineDll();
			})

		
		void GraphicsSystem::Render()
		{
			for (auto& _renderPass : m_RenderPassList)
			{

			}
		}

		void GraphicsSystem::Initialize()
		{
			LoadGraphicsEngineDll();
		}

		void GraphicsSystem::LoadGraphicsEngineDll()
		{
			std::string _moduleName = "GraphicsEngine";

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

			g_GraphicsEngineModule = LoadLibraryA(_moduleName.c_str());

			assert(g_GraphicsEngineModule != NULL);

			using CreateGraphicsEngine = Graphics::GraphicsEngine * (*)(Graphics::GraphicsEngineDesc&);

			auto createGraphicsEngine = (CreateGraphicsEngine)LoadProcedure("CreateGraphicsEngine");

			Graphics::GraphicsEngineDesc _desc;

			WindowInfo& _windowInfo = Window::GetInstance()->GetWindowInfo();

			_desc._handle = reinterpret_cast<long>(_windowInfo._hWnd);

			_desc._isFullScreen = false;
			_desc._height = _windowInfo._height;
			_desc._width = _windowInfo._width;

			_desc._renderSystemDesc._systemType = Graphics::SystemType::DirectX11;

			auto ss = createGraphicsEngine(_desc);

			m_GraphicsEngine = std::make_unique<Graphics::GraphicsEngine>(*ss);
		}

		void GraphicsSystem::FreeGraphicsEngineDll()
		{
			using ReleaseModule = void (*)(Graphics::GraphicsEngine*);

			auto releaseRenderSystem = (ReleaseModule)LoadProcedure("ReleaseGraphicsEngine");

			releaseRenderSystem(m_GraphicsEngine.get());

			m_GraphicsEngine.reset();
		}

	}
}