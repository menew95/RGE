#pragma once

#include "GraphicsModule/Core/RenderSystem.h"

namespace Graphics
{
	enum class SystemType
	{
		DirectX11,
		DirectX12,
	};

	struct RenderSystemDesc
	{
		SystemType _systemType = SystemType::DirectX11;

	};

	extern "C"
	{
		GRAPHICS_DLL_DECLSPEC RenderSystem* CreateRenderSystem(RenderSystemDesc& desc);
		GRAPHICS_DLL_DECLSPEC void ReleaseRenderSystem(RenderSystem* renderSystem);
	}

	/*class Module
	{
	public:
		Module(RenderSystemDesc& desc);
		~Module();

		std::string GetModuleFilename(const char* moduleName);

		void LoadDLL();
		void* LoadProcedure(const char* procedureName);
	private:
		RenderSystemDesc m_Desc;

		CreateModule m_ModuleCreate;
		ReleaseModule m_ModuleRelease;
		HINSTANCE m_GraphicsModule;
	};*/
}