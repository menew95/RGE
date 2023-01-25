#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\System\RendererComponents.h"

#include "GameEngine\Core\Component\Component.h"
#include "GameEngine\Core\Component\Renderer.h"

namespace GameEngine
{
	namespace Core
	{

		RendererComponents::RendererComponents(uint32 order, const tstring& componentName)
			: Components(order, componentName)
		{

		}

		RendererComponents::~RendererComponents()
		{

		}

		void RendererComponents::RenderComponents()
		{
			for (auto& _componentIter : m_Components)
			{
				if (_componentIter->GetEnable() && _componentIter->GetGameObject()->GetActiveInHierarchy())
				{
					auto* _renderer = reinterpret_cast<Renderer*>(_componentIter.get());
					_renderer->Render();
				}
			}
		}
	}
}