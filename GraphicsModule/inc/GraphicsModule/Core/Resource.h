#pragma once

#include "ResourceFlags.h"


namespace Graphics
{
    class Resource
    {
    public:
        Resource() = default;

        virtual ResourceType GetResourceType() const = 0;
    };


}