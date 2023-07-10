/**

    @file      RenderManager.h
	@brief     여러 렌더러를 소유 및 렌더 패스를 실행 하는 렌더 매니저
    @author    LWT
    @date      21.06.2023

**/
#pragma once

#include "GraphicsEngine/Renderer/RendererBase.h"

namespace Graphics
{
    class RenderSystem;
    class ResourceManager;
    class CommandBuffer;
    class Renderer;
    class InstanceRenderer;

    class RenderManager
    {
    public:
        RenderManager(RenderSystem* renderSystem, ResourceManager* resourceManager, CommandBuffer* commandBuffer);

        ~RenderManager();

        /**
            @brief 렌더링 하기전에 필요한 작업들을 실행 
        **/
        void Begin();

        /**
            @brief 등록된 렌더 큐들을 실행
            @param commandBuffer - 컨테스트
        **/
        void Excute(CommandBuffer* commandBuffer);

        /**
            @brief 렌더링이 끝난후 작업들을 실행
        **/
        void End();

    private:

        void InitRenderer();

        RenderSystem* m_RenderSystem;

        ResourceManager* m_ResourceManager;

        CommandBuffer* m_CommandBuffer;

        std::shared_ptr<Renderer> m_DefualtRenderer;

        std::shared_ptr<InstanceRenderer> m_InstanceRenderer;

        std::vector<RenderPass*> m_RenderPassList;
    };
}