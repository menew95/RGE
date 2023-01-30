#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Resource\Material.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GraphicsEngine/Resource/MaterialBuffer.h"

namespace GameEngine
{
	namespace Core
	{

		Material::Material(const tstring& _typeName /*= TEXT("Material")*/)
		{

		}

		Material::~Material()
		{
			Release();
		}

		void Material::SetAlbedoTexture(Graphics::Texture* texture)
		{
			m_AlbedoMap = texture;
			//m_MaterialBuffer->SetResource(4, (Graphics::Resource*)texture);
		}

		void Material::SetNormalTexture(Graphics::Texture* texture)
		{
			m_NormalMap = texture;
			//m_MaterialBuffer->SetResource(5, (Graphics::Resource*)texture);
		}

		void Material::SetMRATexture(Graphics::Texture* texture)
		{
			m_MRAMap = texture;
			//m_MaterialBuffer->SetResource(6, (Graphics::Resource*)texture);
		}

		void Material::Release()
		{
			GraphicsSystem::GetInstance()->DeleteMaterialBuffer(m_MaterialBuffer);
		}
	}
}