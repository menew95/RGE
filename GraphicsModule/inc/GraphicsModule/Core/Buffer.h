#pragma once

#include "GraphicsModule/Core/Resource.h"
#include "GraphicsModule/Core/BufferFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC Buffer : public Resource
	{
    public:
        Buffer(uint32 bindFlags);
        virtual ~Buffer() = default;

        ResourceType GetResourceType() const override final;

        inline long GetBindFlags() const
        {
            return m_BindFlags;
        }

        
    protected:

        Buffer(long bindFlags);

    private:

        long m_BindFlags = 0;
	};
}