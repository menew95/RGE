/**

    @file      RenderManager.h
    @brief     여러 렌더러를 관리하는 매니저
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

    class RenderManager
    {
    public:
        RenderManager(RenderSystem* renderSystem, ResourceManager* resourceManager, CommandBuffer* commandBuffer);

        ~RenderManager();
        
        void Excute(CommandBuffer* commandBuffer);

    private:

        void InitRenderer();

        RenderSystem* m_RenderSystem;

        ResourceManager* m_ResourceManager;

        CommandBuffer* m_CommandBuffer;

        std::shared_ptr<RendererBase> m_InstanceRenderer;

    };
}