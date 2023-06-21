/**

    @file      InstanceRenderer.h
    @brief     Instance Renderer
    @details   ~
    @author    LWT
    @date      15.06.2023
    
**/
#pragma once

#include "GraphicsEngine/Renderer/RendererBase.h"

#include "GraphicsEngine/RenderObject.h"

namespace Graphics
{
    class RenderSystem;
    class ResourceManager;
    class InstanceBuffer;

    class InstanceRenderer final : public RendererBase
    {
    public:
        InstanceRenderer(RenderSystem* renderSystem, ResourceManager* resourceManager);
        
        ~InstanceRenderer();

        void RegistInstanceQueue(const RenderData* renderData);

        void Excute();

    private:

        void Initialize();

        void UpdateInstanceBuffer();

        RenderSystem* m_RenderSystem;

        ResourceManager* m_ResourceManager;

        std::shared_ptr<InstanceBuffer> m_InstanceBuffer;

        std::unordered_map<uuid, MaterialInstanceData> m_InstanceQueue;

    };

}