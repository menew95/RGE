#pragma once

#include "GraphicsModule/Core/RenderSystemObject.h"

#include "GraphicsModule/Core/ResourceFlags.h"

namespace Graphics
{
    class GRAPHICS_DLL_DECLSPEC Resource : public RenderSystemObject
    {
    public:
        Resource() = default;

        virtual ResourceType GetResourceType() const = 0;
    };


}