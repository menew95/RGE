/**

    @file      RendererBase.h
    @brief     render interface
    @details   ~
    @author    LWT
    @date      14.06.2023

**/
#pragma once

#include "Common.h"

namespace Graphics
{
	class RenderSystem;
	class ResourceManager;
	class CommandBuffer;
	class RenderPass;

    struct MaterialInstance;
    struct MeshInstance;

    class RendererBase
    {
    public:
        //virtual void BindRenderPass() abstract;
        //
        //virtual void BindFrameBuffer() abstract;
        //
        //virtual void BindResource() abstract;
    };
}