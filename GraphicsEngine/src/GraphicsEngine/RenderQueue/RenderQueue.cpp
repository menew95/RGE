#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderQueue/RenderQueue.h"

namespace Graphics
{
	void RenderQueue::Push(RenderPass* pass, RenderData& renderData)
	{
		if (m_RenderDataQueue.contains(pass))
		{
			m_RenderDataQueue[pass].push(renderData);
		}
		else
		{
			std::queue<RenderData> _queue;
			_queue.push(renderData);

			m_RenderDataQueue.insert(std::make_pair(pass, _queue));
		}
	}

}