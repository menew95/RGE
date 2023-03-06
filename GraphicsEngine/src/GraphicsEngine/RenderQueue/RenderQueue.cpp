#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderQueue/RenderQueue.h"

namespace Graphics
{

	RenderQueue::RenderQueue()
	{

	}

	RenderQueue::~RenderQueue()
	{

	}

	void RenderQueue::RegistRenderQueue(RenderObject& renderObject)
	{
		m_RenderQueue.push(renderObject);
	}
}