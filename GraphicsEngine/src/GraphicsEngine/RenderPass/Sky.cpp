#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/RenderPass/Sky.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

namespace Graphics
{
	Sky::Sky(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
	{
		Initialize();
	}

	Sky::~Sky()
	{

	}

	void Sky::ExcutePass()
	{
		m_SkyBox_Pass->BeginExcute(m_CommandBuffer, nullptr);

		m_SkyBox_Pass->Excute(m_CommandBuffer);

		m_SkyBox_Pass->EndExcute(m_CommandBuffer);
	}

	void Sky::SetRenderTarget(RenderTarget* renderTarget)
	{
		m_SkyBox_Pass->SetRenderTarget(renderTarget);
	}

	void Sky::Initialize()
	{
		m_SkyBox_Material = m_ResourceManager->CreateMaterialBuffer(TEXT("SkyBox"));

		m_SkyBox_Mesh = m_ResourceManager->CreateMeshBuffer(TEXT("Cube_Mesh"));

		std::vector<Math::Vector3> _data =
		{
			{ -1.0f, +1.0f, -1.0f },
			{ +1.0f, +1.0f, -1.0f },
			{ +1.0f, +1.0f, +1.0f },
			{ -1.0f, +1.0f, +1.0f },
			{ -1.0f, -1.0f, -1.0f },
			{ +1.0f, -1.0f, -1.0f },
			{ +1.0f, -1.0f, +1.0f },
			{ -1.0f, -1.0f, +1.0f }
		};

		std::vector<uint32> triangles =
		{
			// +x
			1, 5, 2,
			2, 5, 6,
			// -x
			0, 3, 7,
			0, 7, 4,
			// +y
			0, 2, 3,
			0, 1, 2,
			// -y
			4, 6, 5,
			4, 7, 6,
			// +z
			2, 7, 3,
			2, 6, 7,
			// -z
			0, 4, 1,
			1, 4, 5
		};

		uint32 _size = static_cast<uint32>(sizeof(Math::Vector3) * _data.size());

		m_SkyBox_Mesh->CreateVertexBuffer(TEXT("Cube_Mesh"), _data.data(), _size, sizeof(Math::Vector3));
		m_SkyBox_Mesh->CreateSubMesh(TEXT("Cube_Mesh"), triangles);

		auto* _state = m_ResourceManager->GetPipelineState(TEXT("SkyBox"));
		auto* _layout = m_ResourceManager->GetPipelineLayout(TEXT("SkyBox_Layout"));

		std::vector<AttachmentClear> _attachmentClears =
		{
			{ { 1, 0, 0, 0 }, 0 },
			{ 1, 0 }
		};

		m_SkyBox_Pass = m_ResourceManager->GetRenderPass(TEXT("Skybox Pass"));

		RenderObject _skyBoxObject;
		_skyBoxObject.m_MeshBuffer = m_SkyBox_Mesh;
		_skyBoxObject.m_MaterialBuffers.emplace_back(m_SkyBox_Material);

		m_RenderObjects.push_back(_skyBoxObject);

		m_SkyBox_Pass->RegistRenderObject(&m_RenderObjects[0]);
	}
}