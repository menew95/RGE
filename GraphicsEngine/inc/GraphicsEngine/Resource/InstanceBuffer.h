/**

    @file      InstanceBuffer.h
    @brief     인스턴싱 데이터를 모으기 위한 버퍼
    @author    LWT
    @date      15.06.2023

**/
#pragma once

#include "GraphicsEngine/Resource/ByteBuffer.h"

namespace Graphics
{
    class RenderSystem;

    enum class InstanceType
    {
        Static = 0,
        Skin = 1
    };

    class GRAPHICSENGINE_DLL_DECLSPEC InstanceBuffer final : public ByteBuffer
    {
    public:
        InstanceBuffer(Graphics::RenderSystem* renderSystem, uuid uuid);

        ~InstanceBuffer();

        void SetInstanceData(void* src, void* matsrc, uint32 offset);
        void SetInstanceData(void* src, void* skin, void* matsrc, uint32 offset);

    private:
        Buffer* m_InstanceBuffer;

    };
}
