/**

    @file      Renderer.h
    @brief     인스턴스를 하지않는 기본 렌더러
    @author    LWT
    @date      30.06.2023

**/
#pragma once

#include "GraphicsEngine/Renderer/RendererBase.h"

namespace Graphics
{
    struct RenderData;

    class Renderer : public RendererBase
    {
    public:
        Renderer(RenderSystem* renderSystem, ResourceManager* resourceManager);
        virtual ~Renderer();

        /**
            @brief 렌더 패스를 파이프라인에 바인드
            @param renderPass - 바인드할 렌더 패스
        **/
        void BindRenderPass(CommandBuffer* commandBuffer, RenderPass* renderPass);

        /**
            @brief 파이프 라인을 그리기전 처리
        **/
        void BeginExcute(CommandBuffer* commandBuffer);

        /**
			@brief 렌더 데이터를 순회하며 명령 실행
			@param commandBuffer - 컨텍스트
			@param renderPass    - 사용될 렌더 패스
			@param renderDatas   - 렌더 데이터 리스트
        **/
        void Excute(CommandBuffer* commandBuffer, RenderPass* renderPass, std::vector<RenderData>& renderDatas);

        /**
            @brief 파이프라인을 그린후 처리
        **/
        void EndExcute(CommandBuffer* commandBuffer);


    protected:

		RenderSystem* m_RenderSystem;

		ResourceManager* m_ResourceManager;

    };
}