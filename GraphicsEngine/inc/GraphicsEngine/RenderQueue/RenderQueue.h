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

        RenderQueue(const RenderQueue&& other)
            : m_RenderDataQueue(std::move(other.m_RenderDataQueue))
        {}

        ~RenderQueue() = default;

        inline void Push(RenderData& renderData)
        {
            m_RenderDataQueue.push(std::move(renderData)); 
        }

        inline auto Pop() 
        { 
            auto& _ret = m_RenderDataQueue.front();

            m_RenderDataQueue.pop();

            return _ret;
        }

        inline std::queue<RenderData>& GetRenderQueue()
        {
            return m_RenderDataQueue; 
        }

        RenderQueue& operator=(const RenderQueue& other);

        RenderQueue& operator=(RenderQueue&& other);

    private:
        std::queue<RenderData> m_RenderDataQueue;

    };

	RenderQueue& RenderQueue::operator=(const RenderQueue& other)
	{
		this->m_RenderDataQueue = other.m_RenderDataQueue;
		return *this;
	}

    RenderQueue& RenderQueue::operator=(RenderQueue&& other)
    {
        this->m_RenderDataQueue = std::move(other.m_RenderDataQueue);
        return *this;
    }
}