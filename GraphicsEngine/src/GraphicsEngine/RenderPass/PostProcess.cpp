#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/RenderPass/PostProcess.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

#include "GraphicsEngine/Resource/ResourceManager.h"

#include "GraphicsModule/Core/RenderTarget.h"

#include "GraphicsEngine/Resource/ConstantBuffer.h"

namespace Graphics
{
	PostProcess::PostProcess(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
	{
		Initialize();
	}

	PostProcess::~PostProcess()
	{

	}

	void PostProcess::ExcutePass()
	{
		m_PostProcess_Pass->BeginExcute(m_CommandBuffer, nullptr);

		m_PostProcess_Pass->Excute(m_CommandBuffer);

		m_PostProcess_Pass->EndExcute(m_CommandBuffer);
	}

	void PostProcess::SetRenderTarget(RenderTarget* renderTarget)
	{

	}

	void PostProcess::Initialize()
	{
		std::vector<AttachmentClear> _attachmentClears =
		{
			{ { 0, 0, 0, 0 }, 0 },
		};

		m_PostProcess_Pass = m_ResourceManager->GetRenderPass(TEXT("SSReflect Pass"));
		m_PostProcess_Pass->SetAttachmentClears(_attachmentClears);

		m_RenderObjects.m_MeshBuffer = m_ResourceManager->GetMeshBuffer(TEXT("Screen_Mesh"));
		m_RenderObjects.m_MaterialBuffer = m_ResourceManager->GetMaterialBuffer(TEXT("Light"));

		static SSLR _sslr;
		_sslr.cb_depthBufferSize = { 1280, 720 };
		_sslr.cb_zThickness = 0.1f;

		_sslr.cb_nearPlaneZ = 0.1f;
		_sslr.cb_maxSteps = 300.f;
		_sslr.cb_maxDistance = 100.f;
		_sslr.cb_strideZCutoff = 1.f;
		_sslr.cb_stride = 1.f;

		UpdateResourceData _perDraw{ eUpdateTime::PerObject, 1, ResourceType::Buffer, &_sslr, sizeof(SSLR) };
		m_RenderObjects.m_UpdateResourcePerObjects.push_back(_perDraw);

		m_PostProcess_Pass->RegistRenderObject(&m_RenderObjects);
	}
}