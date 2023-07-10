/**

    @file      RenderQueue.h
    @brief     렌더링 할 데이터를 모아둔 큐
    @author    LWT
    @date      23.06.2023

**/
#pragma once

#include "GraphicsEngine/RenderObject.h"

namespace Graphics
{
    class RenderQueue
    {
    public:
        RenderQueue() = default;

        RenderQueue(const RenderQueue& other) = default;

        RenderQueue(const RenderQueue&& other) noexcept
            : m_RenderDataQueue(other.m_RenderDataQueue)
        {}

        ~RenderQueue() = default;

        void Push(MaterialBuffer* pass, RenderData& renderData);

		inline std::unordered_map<MaterialBuffer*, std::queue<RenderData>>& GetRenderQueues()
		{
			assert(m_RenderDataQueue.contains(pass));

			return m_RenderDataQueue[pass];
		}

        inline std::queue<RenderData>& GetRenderQueue(RenderPass* pass)
        {
            assert(m_RenderDataQueue.contains(pass));

            return m_RenderDataQueue[pass]; 
        }

        inline RenderQueue& operator=(const RenderQueue& other) noexcept
		{
			this->m_RenderDataQueue = other.m_RenderDataQueue;
			return *this;
		}

        inline RenderQueue& operator=(RenderQueue&& other) noexcept
		{
			this->m_RenderDataQueue = std::move(other.m_RenderDataQueue);
			return *this;
		}

    private:
        std::unordered_map<MaterialBuffer*, std::queue<RenderData>> m_RenderDataQueue;

    };
}