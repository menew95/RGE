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

        void BindRenderPass(RenderPass* renderPass);

        void BeginExcute();

        void Excute();

        void EndExcute();

    private:

        /**
            @brief 인스턴스 버퍼 등 필요한 리소스들을 초기화
        **/
        void Initialize();

        /**
            @brief 모아둔 매쉬 인스턴스 정보로 인스턴스 버퍼를 업데이트 해줌
            @param meshInstanceData - 매쉬 인스턴스 데이터
        **/
        void UpdateInstanceBuffer(MeshInstanceData& meshInstanceData);

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