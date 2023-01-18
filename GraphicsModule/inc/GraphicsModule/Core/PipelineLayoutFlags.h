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
			_type{ type },
			_bindFlags{ bindFlags },
			_stageFlags{ stageFlags },
			_slot{ slot },
			_arraySize{ arraySize }
        {
        }

        inline BindingDescriptor(
            const tstring& name,
            ResourceType        type,
            long                bindFlags,
            long                stageFlags,
			uint32       slot,
			uint32       arraySize = 1)
            :
			_name{ name },
			_type{ type },
			_bindFlags{ bindFlags },
			_stageFlags{ stageFlags },
			_slot{ slot },
			_arraySize{ arraySize }
        {
        }

        tstring     _name;

        ResourceType    _type = ResourceType::Undefined;
        long            _bindFlags = 0;
        long            _stageFlags = 0;
        uint32   _slot = 0;
        uint32   _arraySize = 1;
    };

    struct PipelineLayoutDesc
    {
        std::vector<BindingDescriptor> _bindings;
        std::vector<class Resource*> _resources;
    };
}