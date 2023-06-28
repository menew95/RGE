/**

    @file      InstanceRenderer.h
    @brief     Instance Renderer
    @details   ~
    @author    LWT
    @date      15.06.2023
    
**/
#pragma once

#include "GraphicsEngine/Renderer/RendererBase.h"

#include "GraphicsEngine/RenderObject.h"

#define STATIC_INSTANCE_NUM 5230
#define SKIN_INSTANCE_NUM 110

namespace Graphics
{
    class RenderSystem;
    class ResourceManager;
    class CommandBuffer;
    class InstanceBuffer;
    class RenderPass;

    class InstanceRenderer final : public RendererBase
    {
    public:
        InstanceRenderer(RenderSystem* renderSystem, ResourceManager* resourceManager, CommandBuffer* commandBuffer);

        ~InstanceRenderer();

        /**
            @brief 인스턴스 큐에 렌더 데이터를 등록 혹은 추가
            @param renderData - 렌더 오브젝트의 어느 서브 매쉬와 머티리얼을 사용해야지 담고있는 중간 컨테이너
        **/
        void RegistInstanceQueue(const RenderData* renderData);

        /**
            @brief 등록된 인스턴스 큐를 비움
        **/
        void ClearInstanceQueue();

        /**
            @brief
        **/
        void BeginExcute();

        /**
            @brief 등록한 인스턴스 데이터를 순회하며 명령 실행
        **/
        void Excute();

        /**
            @brief
        **/
        void EndExcute();
         
    private:

        /**
            @brief 인스턴스 버퍼 등 필요한 리소스들을 초기화
        **/
        void Initialize();

        /**
            @brief 렌더 패스를 파이프라인에 바인드
            @param renderPass - 바인드할 렌더 패스
        **/
        void BindRenderPass(RenderPass* renderPass);

        /**
            @brief 모아둔 매쉬 인스턴스 정보로 인스턴스 버퍼를 업데이트 해줌
            @param meshInstanceData - 매쉬 인스턴스 데이터
            @param  startNum        - 인스턴스 데이터를 가지고올 시작 인덱스
            @retval                 - 인스턴스 갯수
        **/
        uint32 UpdateInstanceBuffer(MeshInstanceData& meshInstanceData, uint32 startNum);

        RenderPass* GetRenderPass(MaterialBuffer* materialBuffer, bool skin);

        RenderSystem* m_RenderSystem;

        ResourceManager* m_ResourceManager;

        CommandBuffer* m_CommandBuffer;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Pass;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Pass;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Bump_Pass;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Bump_MRA_Pass;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Pass;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Pass;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Bump_Pass;

        std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass;

        std::shared_ptr<InstanceBuffer> m_InstanceBuffer;

        std::unordered_map<uuid, MaterialInstanceData> m_InstanceQueue;
    };
}