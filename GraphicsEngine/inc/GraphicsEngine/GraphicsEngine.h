#pragma once

#include "Common.h"

namespace Graphics
{
	class GraphicsEngine
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();

	private:
		void LoadDllAndCreateRenderSystem();
		void FreeDllAndReleaseRenderSystem();
		void* LoadProcedure(const char* procedureName);

		class RenderSystem* m_RenderSystem;
	};
}