#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

/*
* �Ž����� ��Ƽ� �׸��� �н�
*/

namespace Graphics
{
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

	private:
		void Culling();
		
		CommandBuffer* m_CommandBuffer;

		std::vector<RenderObject> m_RenderObjectList;

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