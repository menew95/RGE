#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

namespace Graphics
{
	class RenderQueue
	{
	public:
		RenderQueue();
		~RenderQueue();

		void RegistRenderQueue(RenderObject& renderObject);

	private:
		std::queue<RenderObject> m_RenderQueue;
	};
}
