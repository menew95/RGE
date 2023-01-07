#pragma once

#include "Common.h"
#include "GraphicsModule/Utility/Export.h"
#include "GraphicsModule/Core/Format.h"
#include "GraphicsModule/Core/ResourceFlags.h"

namespace Graphics
{
    struct BindingDescriptor
    {
        BindingDescriptor() = default;
        BindingDescriptor(const BindingDescriptor&) = default;

        inline BindingDescriptor(
            ResourceType    type,
            long            bindFlags,
            long            stageFlags,
            uint32   slot,
            uint32   arraySize = 1)
            :
            type{ type },
            bindFlags{ bindFlags },
            stageFlags{ stageFlags },
            slot{ slot },
            arraySize{ arraySize }
        {
        }

        inline BindingDescriptor(
            const std::string& name,
            ResourceType        type,
            long                bindFlags,
            long                stageFlags,
			uint32       slot,
			uint32       arraySize = 1)
            :
            name{ name },
            type{ type },
            bindFlags{ bindFlags },
            stageFlags{ stageFlags },
            slot{ slot },
            arraySize{ arraySize }
        {
        }

        std::string     name;

        ResourceType    type = ResourceType::Undefined;
        long            bindFlags = 0;
        long            stageFlags = 0;
        uint32   slot = 0;
        uint32   arraySize = 1;
    };

    struct PipelineLayoutDesc
    {
        std::vector<BindingDescriptor> bindings;
    };
}