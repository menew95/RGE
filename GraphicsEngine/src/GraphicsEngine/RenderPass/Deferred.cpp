#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/RenderPass/Deferred.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

namespace Graphics
{

	Deferred::Deferred(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
		, m_CameraBuffer(nullptr)
	{
		Initialize();
	}

	Deferred::~Deferred()
	{

	}

	void Deferred::ExcutePass()
	{
		Culling();

		m_CommandBuffer->BeginEvent(TEXT("Deferred Pass"));

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

		m_CommandBuffer->EndEvent();

		m_RenderObjectList.clear();
	}

	void Deferred::RegistRenderObject(RenderObject& renderObject)
	{
		m_RenderObjectList.push_back(renderObject);
	}

	void Deferred::Initialize()
	{
		m_Deferred_Mesh_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh Pass"));

		m_Deferred_Mesh_Albedo_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Albedo Pass"));

		m_Deferred_Mesh_Albedo_Bump_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Albedo_Bump Pass"));

		m_Deferred_Mesh_Albedo_Bump_MRA_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Albedo_Bump_MRA Pass"));

		m_Deferred_Mesh_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin Pass"));

		m_Deferred_Mesh_Skinned_Albedo_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Albedo Pass"));

		m_Deferred_Mesh_Skinned_Albedo_Bump_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Albedo_Bump Pass"));

		m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Skin_Albedo_Bump_MRA Pass"));
	}

	void Deferred::Culling()
	{
		assert(m_CameraBuffer != nullptr);

		BoundingFrustum m_Frustum{ m_CameraBuffer->GetProj() };

		m_Frustum.Transform(m_Frustum, m_CameraBuffer->GetWorld());

		int cul = 0;

		for (size_t i = 0; i < m_RenderObjectList.size(); i++)
		{
			// Frustum Culling

			auto _min = m_RenderObjectList[i].m_MeshBuffer->GetBoundingBoxMin();
			auto _max = m_RenderObjectList[i].m_MeshBuffer->GetBoundingBoxMax();

			auto _center = (_min + _max) * 0.5f;
			auto _extents = _max - _center;

			BoundingOrientedBox _boundingOrientedBox;
			_boundingOrientedBox.Center = _center;
			_boundingOrientedBox.Extents = _extents;

			_boundingOrientedBox.Transform(_boundingOrientedBox, m_RenderObjectList[i].m_World);

			if (m_Frustum.Intersects(_boundingOrientedBox))
			{
				switch (m_RenderObjectList[i].m_RenderPassIdx)
				{
					case 0:
					{
						m_Deferred_Mesh_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					case 1:
					{
						m_Deferred_Mesh_Albedo_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					case 2:
					{
						m_Deferred_Mesh_Albedo_Bump_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					case 3:
					{
						m_Deferred_Mesh_Albedo_Bump_MRA_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					case 4:
					{
						m_Deferred_Mesh_Skinned_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					case 5:
					{
						m_Deferred_Mesh_Skinned_Albedo_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					case 6:
					{
						m_Deferred_Mesh_Skinned_Albedo_Bump_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					case 7:
					{
						m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass->RegistRenderObject(m_RenderObjectList[i]);
						break;
					}
					default:
						assert(false);
						break;
				}
			}
			else
			{
				cul++;
			}
		}

		if (cul != 0)
		{
			int a = 0;
		}
	}

}