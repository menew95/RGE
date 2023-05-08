#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/RenderPass/DebugDeferred.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

namespace Graphics
{

	DebugDeferred::DebugDeferred(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
	{
		Initialize();
	}

	DebugDeferred::~DebugDeferred()
	{

	}

	void DebugDeferred::ExcutePass()
	{
		m_Debug_Pass->BeginExcute(m_CommandBuffer, nullptr);

		m_Debug_Pass->Excute(m_CommandBuffer);

		m_Debug_Pass->EndExcute(m_CommandBuffer);
	}

	void DebugDeferred::SetSwapChain(class SwapChain* swapchain)
	{
		m_Debug_Pass->SetRenderTarget(swapchain);
	}

	void DebugDeferred::Initialize()
	{
		auto* _mesh = m_ResourceManager->GetMeshBuffer(TEXT("Screen_Mesh"));

		RenderObject _albedoObject;
		_albedoObject.m_MeshBuffer = _mesh;
		_albedoObject.m_MaterialBuffer = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug_Albedo"));

		_albedoObject.m_MaterialBuffer->SetResource(m_ResourceManager->GetTexture(TEXT("Albedo")), ResourceType::Texture, 0);

		RenderObject _normalObject;
		_normalObject.m_MeshBuffer = _mesh;
		_normalObject.m_MaterialBuffer = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug_Normal"));;

		_normalObject.m_MaterialBuffer->SetResource(m_ResourceManager->GetTexture(TEXT("Normal")), ResourceType::Texture, 0);

		RenderObject _depthObject;
		_depthObject.m_MeshBuffer = _mesh;
		_depthObject.m_MaterialBuffer = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug_Depth"));;

		_depthObject.m_MaterialBuffer->SetResource(m_ResourceManager->GetTexture(TEXT("Depth")), ResourceType::Texture, 0);

		RenderObject _worldPosObject;
		_worldPosObject.m_MeshBuffer = _mesh;
		_worldPosObject.m_MaterialBuffer = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug_WorldPos"));;

		_worldPosObject.m_MaterialBuffer->SetResource(m_ResourceManager->GetTexture(TEXT("WorldPosition")), ResourceType::Texture, 0);

		RenderObject _reflectObject;
		_reflectObject.m_MeshBuffer = _mesh;
		_reflectObject.m_MaterialBuffer = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug_Reflect"));;

		_reflectObject.m_MaterialBuffer->SetResource(m_ResourceManager->GetTexture(TEXT("SSReflect")), ResourceType::Texture, 0);

		RenderObject _voxelObject;
		_voxelObject.m_MeshBuffer = _mesh;
		_voxelObject.m_MaterialBuffer = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug_Voxel"));;

		_voxelObject.m_MaterialBuffer->SetResource(m_ResourceManager->GetTexture(TEXT("VoxelGI")), ResourceType::Texture, 0);

		m_DebugRenderObject.push_back(_albedoObject);
		m_DebugRenderObject.push_back(_normalObject);
		m_DebugRenderObject.push_back(_depthObject);
		m_DebugRenderObject.push_back(_worldPosObject);
		m_DebugRenderObject.push_back(_reflectObject);

		m_Debug_Pass = m_ResourceManager->GetRenderPass(TEXT("MRT Debug Pass"));

		m_Debug_Pass->AddResourceClear({ ResourceType::Texture, 0, 1, BindFlags::ShaderResource, StageFlags::PS });

		for (uint i = 0; i < m_DebugRenderObject.size(); i++)
		{
			Math::Viewport _viewport{ 256.f * i, 0.f, 256.f, 144.f };

			m_DebugRenderObject[i].AddViewport(_viewport);

			m_Debug_Pass->RegistRenderObject(&m_DebugRenderObject[i]);
		}

		m_DebugRenderObject.push_back(_voxelObject);
		m_DebugRenderObject[5].AddViewport({ 0, 144.f, 256.f, 144.f });
		m_Debug_Pass->RegistRenderObject(&m_DebugRenderObject[5]);
	}
}