/**

    @file      InstanceRenderer.h
    @brief     Instance Renderer
    @details   ~
    @author    LWT
    @date      15.06.2023
    
**/
#pragma once

#include "GraphicsEngine/RenderManager/RendererBase.h"

#include "GraphicsEngine/RenderObject.h"

namespace Graphics
{
    class InstanceRenderer final : public RendererBase
    {
    public:
        InstanceRenderer();
        
        ~InstanceRenderer();

        void RegistInstanceQueue(const RenderData* );



    private:

        void UpdateInstanceBuffer();

    };

}