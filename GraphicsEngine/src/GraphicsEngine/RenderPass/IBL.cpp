#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderPass/IBL.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

namespace Graphics
{
	IBL::IBL(CommandBuffer* command, ResourceManager* resourceManager)
		: m_CommandBuffer(command)
	{
		CreateCubeMapMatrix();

		SetResources(resourceManager);
	}

	IBL::~IBL()
	{

	}

	void IBL::CreatePreFilteredMap()
	{
		for (uint32 _mip = 0; _mip < MAXMIPMAP; _mip++)
		{
			CreatePreFilteredMip(_mip);
		}
	}

	void IBL::CreateIrradianceMap()
	{
		RenderObject _renderObject;

		_renderObject.m_MaterialBuffer = m_Cube_Material;
		_renderObject.m_MeshBuffer = m_Cube_Mesh;

		UpdateResourceData _resourceD{ eUpdateTime::PerObject, 2, ResourceType::Buffer, &m_PerCubeMapMatrix, sizeof(PerCubeMapMatrix) };
		_renderObject.m_UpdateResourcePerObjects.push_back(_resourceD);

		m_Irradiance_Pass->RegistRenderObject(_renderObject);

		m_Irradiance_Pass->BeginExcute(m_CommandBuffer, nullptr);

		m_Irradiance_Pass->Excute(m_CommandBuffer);

		m_Irradiance_Pass->EndExcute(m_CommandBuffer);
	}

	void IBL::CreatePreFilteredMip(uint32 mipLevel)
	{
		m_PreFiltered_Pass->SetRenderTarget(m_MipRenderTarget[mipLevel]);

		RenderObject _renderObject;

		_renderObject.m_MaterialBuffer = m_Cube_Material;
		_renderObject.m_MeshBuffer = m_Cube_Mesh;

		UpdateResourceData _resourceD{ eUpdateTime::PerObject, 2, ResourceType::Buffer, &m_PerCubeMapMatrix, sizeof(PerCubeMapMatrix) };
		_renderObject.m_UpdateResourcePerObjects.push_back(_resourceD);

		m_PerPreFiltered._roughness = static_cast<float>(mipLevel) / MAXMIPMAP;

		UpdateResourceData MipRoughenss{ eUpdateTime::PerObject, 3, ResourceType::Buffer, &m_PerPreFiltered, sizeof(float) };
		_renderObject.m_UpdateResourcePerObjects.push_back(MipRoughenss);

		float _size = 512.f * std::powf(0.5, static_cast<float>(mipLevel));

		Math::Viewport _viewport{ 0.f, 0.f, _size, _size, 0.f, 1.0f };
		_renderObject.AddViewport(_viewport);

		m_PreFiltered_Pass->RegistRenderObject(_renderObject);

		m_PreFiltered_Pass->BeginExcute(m_CommandBuffer, nullptr);

		m_PreFiltered_Pass->Excute(m_CommandBuffer);

		m_PreFiltered_Pass->EndExcute(m_CommandBuffer);
	}

	void IBL::SetResources(ResourceManager* resourceManager)
	{
		m_Cube_Material = resourceManager->CreateMaterialBuffer(TEXT("PreFilteredMaterial"));
		m_Cube_Mesh = resourceManager->GetMeshBuffer(TEXT("Cube_Mesh"));

		m_MipRenderTarget[0] = resourceManager->GetRenderTarget(TEXT("Pre_Filtered0"));
		m_MipRenderTarget[1] = resourceManager->GetRenderTarget(TEXT("Pre_Filtered1"));
		m_MipRenderTarget[2] = resourceManager->GetRenderTarget(TEXT("Pre_Filtered2"));
		m_MipRenderTarget[3] = resourceManager->GetRenderTarget(TEXT("Pre_Filtered3"));
		m_MipRenderTarget[4] = resourceManager->GetRenderTarget(TEXT("Pre_Filtered4"));

		m_PreFiltered_Pass = resourceManager->GetRenderPass(TEXT("Pre_Filtered Pass"));
		m_Irradiance_Pass = resourceManager->GetRenderPass(TEXT("Irradiance Pass"));
	}

	void IBL::CreateCubeMapMatrix()
	{
		Math::Vector3 _eye = { 0.0f, 0.0f, 0.0f };

		Math::Vector3 _look, _up;

		// +X
		_look = { 1.0f, 0.0f, 0.0f };
		_up = { 0.0f, 1.0f, 0.0f };
		m_PerCubeMapMatrix._view[0] = Math::Matrix::CreateLookAt(_eye, _look, _up);

		// -X
		_look = { -1.0f, 0.0f, 0.0f };
		_up = { 0.0f, 1.0f, 0.0f };
		m_PerCubeMapMatrix._view[1] = Math::Matrix::CreateLookAt(_eye, _look, _up);

		// +Y
		_look = { 0.0f, 1.0f, 0.0f };
		_up = { 0.0f, 0.0f, -1.0f };
		m_PerCubeMapMatrix._view[2] = Math::Matrix::CreateLookAt(_eye, _look, _up);

		// -Y
		_look = { 0.0f, -1.0f, 0.0f };
		_up = { 0.0f, 0.0f, 1.0f };
		m_PerCubeMapMatrix._view[3] = Math::Matrix::CreateLookAt(_eye, _look, _up);

		// +Z
		_look = { 0.0f, 0.0f, 1.0f };
		_up = { 0.0f, 1.0f, 0.0f };
		m_PerCubeMapMatrix._view[4] = Math::Matrix::CreateLookAt(_eye, _look, _up);

		// -Z
		_look = { 0.0f, 0.0f, -1.0f };
		_up = { 0.0f, 1.0f, 0.0f };
		m_PerCubeMapMatrix._view[5] = Math::Matrix::CreateLookAt(_eye, _look, _up);

		m_PerCubeMapMatrix._proj = Math::Matrix::CreatePerspectiveFieldOfView(90.f * Math::Deg2Rad, 1.0, 1.0f, 100.f);
	}

	void IBL::CreateIBLResource()
	{
		CreatePreFilteredMap();

		CreateIrradianceMap();
	}

}