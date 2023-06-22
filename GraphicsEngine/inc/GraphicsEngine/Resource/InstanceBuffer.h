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
        InstanceBuffer(Graphics::RenderSystem* renderSystem, uuid uuid, uint32 size = 5000);

        ~InstanceBuffer();

        /**
            @brief 스태틱 매쉬의 인스턴스 데이터를 버퍼에 기록
            @param src    - 트랜스폼 인스턴스 데이터
            @param matsrc - 머티리얼 인스턴스 데이터
            @param offset - 버퍼에 기록할 오프셋 위치
        **/
        void SetInstanceData(void* src, void* matsrc, uint32 offset);

        /**
            @brief 스킨 매쉬의 인스턴스 데이터를 버퍼에 기록
            @param src    - 트랜스폼 인스턴스 데이터
            @param skin   - 본 트랜스폼 인스턴스 데이터
            @param matsrc - 머티리얼 인스턴스 데이터
            @param offset - 버퍼에 기록할 오프셋 위치
        **/
        void SetInstanceData(void* src, void* skin, void* matsrc, uint32 offset);

        /**
            @brief 버퍼에 데이터를 업데이트
            @param commandBuffer - 커맨드 버퍼
        **/
        void UpdateInstanceBuffer(CommandBuffer* commandBuffer);

        /**
            @brief 파이프 라인에 바인드
            @param commandBuffer - 커맨드 퍼버
            @param stageFlags    - 셰이더 스테이지 플레그
        **/
        void BindBuffer(CommandBuffer* commandBuffer, uint32 slot, uint32 stageFlags);

        inline Buffer* GetInstanceBuffer() const { return m_InstanceBuffer; }

    private:

        /**
            @brief 버퍼 크기에 맞게 재생성
            @param size - 크기
        **/
        void ResizeBuffer(uint32 size);

        Buffer* m_InstanceBuffer;

        Buffer* m_SkinInstanceBuffer;

        uint32 m_BufferSize;
    };
}
