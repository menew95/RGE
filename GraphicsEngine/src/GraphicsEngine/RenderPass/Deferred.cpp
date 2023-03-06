#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/RenderPass/Deferred.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"

namespace Graphics
{

	Deferred::Deferred(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
	{

	}

	Deferred::~Deferred()
	{

	}

	void Deferred::ExcutePass()
	{
		{
			m_Deferred_Mesh_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Albedo_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Albedo_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Albedo_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Albedo_Bump_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Albedo_Bump_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Albedo_Bump_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Albedo_Bump_MRA_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Albedo_Bump_MRA_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Albedo_Bump_MRA_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Skinned_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Skinned_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Skinned_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Skinned_Albedo_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Skinned_Albedo_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Skinned_Albedo_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Skinned_Albedo_Bump_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Skinned_Albedo_Bump_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Skinned_Albedo_Bump_Pass->EndExcute(m_CommandBuffer);
		}

		{
			m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass->BeginExcute(m_CommandBuffer, nullptr);

			m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass->Excute(m_CommandBuffer);

			m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass->EndExcute(m_CommandBuffer);
		}
	}

	void Deferred::RegistRenderObject(RenderObject& renderObject)
	{
		m_RenderObjectList.push_back(renderObject);
	}

	void Deferred::Culling()
	{
		BoundingFrustum m_Frustum;

		for (size_t i = 0; i < m_RenderObjectList.size(); i++)
		{
			// Frustum Culling

			auto _min = m_RenderObjectList[i].m_MeshBuffer->GetBoundingBoxMin();
			auto _max = m_RenderObjectList[i].m_MeshBuffer->GetBoundingBoxMax();

			auto _center = (_min + _max) * 0.5f;
			auto _extents = _max - _center;
			auto _world = m_RenderObjectList[i].m_World;

			BoundingOrientedBox _boundingOrientedBox;
			_boundingOrientedBox.Center = _center;
			_boundingOrientedBox.Extents = _extents;

			_boundingOrientedBox.Transform(_boundingOrientedBox, _world);



		}
	}

}