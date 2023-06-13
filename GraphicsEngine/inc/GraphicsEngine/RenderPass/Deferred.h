#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

/*
* 매쉬들을 모아서 그리는 패스
*/

namespace Graphics
{
	class CameraBuffer;
	class RenderPass;
	class CommandBuffer;
	class ResourceManager;

	class Deferred
	{
	public:
		Deferred(CommandBuffer* command, ResourceManager* resourceManager);
		~Deferred();

		void ExcutePass();

		void RegistRenderObject(RenderObject& renderObject);
		void RegistRenderObject(RenderObject* renderObject);

		inline void SetCameraBuffer(CameraBuffer* camera) { m_CameraBuffer = camera; }

	private:
		void Initialize();

		void Culling();
		
		ResourceManager* m_ResourceManager;
		CommandBuffer* m_CommandBuffer;

		CameraBuffer* m_CameraBuffer;

		std::vector<RenderObject*> m_RenderObjectList;

		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Bump_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Bump_MRA_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Bump_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass;
	};
}