#pragma once

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/ResourceFlags.h"

namespace Graphics
{
    class GRAPHICS_DLL_DECLSPEC Resource
    {
    public:
        Resource() = default;

        virtual ResourceType GetResourceType() const = 0;
    };


}