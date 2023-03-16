#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/RenderPass/DebugDeferred.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

namespace Graphics
{

	DebugDeferred::DebugDeferred(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
		, m_Screen_Mesh(nullptr)
		, m_Debug_Material(nullptr)
		, m_Albedo(nullptr)
		, m_Normal(nullptr)
		, m_Depth(nullptr)
		, m_World(nullptr)
		, m_Reflect(nullptr)
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
		m_Debug_Material = m_ResourceManager->CreateMaterialBuffer(TEXT("MRT_Debug"));

		m_Screen_Mesh = m_ResourceManager->GetMeshBuffer(TEXT("Screen_Mesh"));

		m_Albedo = m_ResourceManager->GetTexture(TEXT("Albedo"));
		m_Normal = m_ResourceManager->GetTexture(TEXT("Normal"));
		m_Depth = m_ResourceManager->GetTexture(TEXT("Depth"));
		m_World = m_ResourceManager->GetTexture(TEXT("WorldPosition"));
		m_Reflect = m_ResourceManager->GetTexture(TEXT("SSReflect"));

		RenderObject _albedoObject;
		_albedoObject.m_MeshBuffer = m_Screen_Mesh;
		_albedoObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceA{ 1,  m_Albedo };
		_albedoObject.m_UpdateResources.push_back(_resourceA);

		RenderObject _normalObject;
		_normalObject.m_MeshBuffer = m_Screen_Mesh;
		_normalObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceN{ 1,  m_Normal };
		_normalObject.m_UpdateResources.push_back(_resourceN);

		RenderObject _depthObject;
		_depthObject.m_MeshBuffer = m_Screen_Mesh;
		_depthObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceD{ 1,  m_Depth };
		_depthObject.m_UpdateResources.push_back(_resourceD);

		RenderObject _worldPosObject;
		_worldPosObject.m_MeshBuffer = m_Screen_Mesh;
		_worldPosObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceW{ 1,  m_World };
		_worldPosObject.m_UpdateResources.push_back(_resourceW);

		RenderObject _reflectObject;
		_reflectObject.m_MeshBuffer = m_Screen_Mesh;
		_reflectObject.m_MaterialBuffer = m_Debug_Material;

		UpdateResourceData _resourceR{ 1,  m_Reflect };
		_reflectObject.m_UpdateResources.push_back(_resourceR);

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
	}
}