#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\System\RenderPipeLineSystem.h"

namespace GameEngine
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(RenderPipeLineSystem, {}, {})

		
		void RenderPipeLineSystem::Render()
		{
			for (auto& _renderPipeLine : m_RenderPipeLineList)
			{
				//_renderPipeLine->BeginRender();
				//_renderPipeLine->Render();
				//_renderPipeLine->EndRender();
			}
		}

	}
}