#include <windows.h>
#include "GraphicsPCH.h"
#include "GraphicsModule/Module.h"

#include "GraphicsModule/Core/DX11/DX11RenderSystem.h"

namespace Graphics
{
	GRAPHICS_DLL_DECLSPEC RenderSystem* CreateRenderSystem(RenderSystemDesc& desc)
	{
		switch (desc._systemType)
		{
			case SystemType::DirectX11: return new DX11::DX11RenderSystem();
			case SystemType::DirectX12:
			default:
				assert(false);
				break;
		}

		return nullptr;
	}

	GRAPHICS_DLL_DECLSPEC void ReleaseRenderSystem(RenderSystem* renderSystem)
	{
		if (renderSystem != nullptr)
		{
			delete renderSystem;
		}
	}

//	Module::Module(RenderSystemDesc& desc)
//		: m_Desc(desc)
//	{
//
//	}
//
//	Module::~Module()
//	{
//		FreeLibrary(m_GraphicsModule);
//	}
//
//	std::string Module::GetModuleFilename(const char* moduleName)
//	{
//		std::string _name = moduleName;
//
//#ifdef _DEBUG
//#ifdef x64
//		_name += "_Debug_x64.dll";
//#else
//		_name += "_Debug_x86.dll";
//#endif // x64
//#else
//#ifdef x64
//		_name += "_Release_x64.dll";
//#else
//		_name += "_Release_x64.dll";
//#endif // x64
//#endif
//	}
//
//	void Module::LoadDLL()
//	{
//		m_GraphicsModule = LoadLibraryA(GetModuleFilename("GraphicsModule").c_str());
//
//		assert(m_GraphicsModule != NULL);
//
//		m_ModuleCreate = (CreateModule)LoadProcedure("CreateRenderSystem");
//
//		assert(m_ModuleCreate != NULL);
//
//		m_ModuleRelease = (ReleaseModule)LoadProcedure("ReleaseRenderSystem");
//
//		assert(m_ModuleRelease != NULL);
//	}
//
//	void* Module::LoadProcedure(const char* procedureName)
//	{
//		/* Get procedure address from library module and return it as raw-pointer */
//		auto procAddr = GetProcAddress(m_GraphicsModule, procedureName);
//		return reinterpret_cast<void*>(procAddr);
//	}

}