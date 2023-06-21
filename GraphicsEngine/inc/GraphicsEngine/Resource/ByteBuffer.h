/**

    @file      ByteBuffer.h
    @brief     ByteBuffer
    @author    LWT
    @date      15.06.2023
**/
#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

namespace Graphics
{
    class RenderSystem;

    class GRAPHICSENGINE_DLL_DECLSPEC ByteBuffer : public ResourceBuffer
    {
    public:
        ByteBuffer(uint32 bufferSize, uuid uuid);

        virtual ~ByteBuffer();

        void UpdateBuffer(void* src, uint32 srcSize, uint32 startPoint = 0);
        
        void ResizeBuffer(uint32 size);

        inline  byte* GetByteBuffer() const { return m_ByteBuffer; }

        inline uint32 GetSize() const { return m_BufferSize; }

    protected:
        
        uint32 m_BufferSize;

        byte* m_ByteBuffer;
    };
}
