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
		//m_PostProcess_Pass->SetRenderTarget(m_RenderTarget[m_CurrIdx]);
		
		m_PostProcess_Pass->BeginExcute(m_CommandBuffer, nullptr);

		m_PostProcess_Pass->Excute(m_CommandBuffer);

		m_PostProcess_Pass->EndExcute(m_CommandBuffer);

		//m_CurrIdx = !m_CurrIdx;

		m_PostProcess_Pass2->SetRenderTarget(m_RenderTarget[m_CurrIdx]);

		m_PostProcess_Pass2->BeginExcute(m_CommandBuffer, nullptr);

		m_PostProcess_Pass2->Excute(m_CommandBuffer);

		m_PostProcess_Pass2->EndExcute(m_CommandBuffer);
	}

	void PostProcess::SetRenderTarget(RenderTarget* renderTarget)
	{

	}

	void PostProcess::SetPostProcessSetting(PostProcessSetting setting)
	{
		Buffer* m_PostProcessCB = m_ResourceManager->GetBuffer(TEXT("PostProcess"));

		assert(m_PostProcessCB != nullptr);

		m_PostProcessSetting = setting;

		m_CommandBuffer->UpdateBuffer(*m_PostProcessCB, 0, &m_PostProcessSetting, sizeof(PostProcessSetting));
	}

	void PostProcess::Initialize()
	{
		TextureDesc _textureDesc;
		_textureDesc._textureType = TextureType::Texture2D;
		_textureDesc._bindFlags = BindFlags::ShaderResource | BindFlags::RenderTarget;
		_textureDesc._format = Format::R32G32B32A32_FLOAT;
		_textureDesc._extend = { 1280, 720, 0 };
		_textureDesc._arrayLayers = 1;
		_textureDesc._mipLevels = 1;
		_textureDesc._samples = 1;

		m_Buffer[0] = m_ResourceManager->CreateTexture(TEXT("PostProcess BackBuffer0"), _textureDesc);
		m_Buffer[1] = m_ResourceManager->CreateTexture(TEXT("PostProcess BackBuffer1"), _textureDesc);

		RenderTargetDesc _rtDesc;
		_rtDesc._extend = { 1280, 720 };
		_rtDesc._sample = 1;

		AttachmentDesc _attDesc;
		_attDesc._renderTargetType = RenderTargetType::RenderTarget;
		_attDesc._resource = m_Buffer[0];
		_attDesc._mipLevel = 0;

		_rtDesc._attachments.push_back(_attDesc);

		m_RenderTarget[0] = m_ResourceManager->CreateRenderTarget(TEXT("PostProcess RT0"), _rtDesc);

		_rtDesc._attachments[0]._resource = m_Buffer[1];

		m_RenderTarget[1] = m_ResourceManager->CreateRenderTarget(TEXT("PostProcess RT1"), _rtDesc);

		m_RenderObjects.m_MeshBuffer = m_ResourceManager->GetMeshBuffer(TEXT("Screen_Mesh"));
		m_RenderObjects.m_MaterialBuffer = m_ResourceManager->GetMaterialBuffer(TEXT("Light"));

		{
			std::vector<AttachmentClear> _attachmentClears =
			{
				{ { 0, 0, 0, 0 }, 0 },
			};

			m_PostProcess_Pass = m_ResourceManager->GetRenderPass(TEXT("SSReflect Pass"));

			m_PostProcess_Pass->SetAttachmentClears(_attachmentClears);

			m_PostProcess_Pass->RegistRenderObject(&m_RenderObjects);

			//m_PostProcess_Pass->AddResourceClear({ ResourceType::Texture, 0, 1, BindFlags::ShaderResource, StageFlags::PS });
		}


		{
			std::vector<AttachmentClear> _attachmentClears =
			{
				{ { 0, 0, 0, 0 }, 0 },
			};

			m_PostProcess_Pass2 = m_ResourceManager->GetRenderPass(TEXT("ToneMap Pass"));
			
			m_PostProcess_Pass2->SetAttachmentClears(_attachmentClears);

			m_PostProcess_Pass2->RegistRenderObject(&m_RenderObjects);
		}

	}
}