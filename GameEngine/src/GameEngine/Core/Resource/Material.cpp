#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Resource\Material.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GraphicsEngine/Resource/MaterialBuffer.h"

#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	registration::class_<GameEngine::Core::Material>("Material")
	.constructor<const tstring&>();
}


namespace GameEngine
{
	namespace Core
	{
		Material::Material(const tstring& _typeName /*= TEXT("Material")*/)
			: Resource(_typeName)
		{

		}

		Material::~Material()
		{
			Release();
		}

		void Material::SetAlbedoTexture(Graphics::Texture* texture)
		{
			m_AlbedoMap = texture;
		}

		void Material::SetNormalTexture(Graphics::Texture* texture)
		{
			m_NormalMap = texture;
		}

		void Material::SetMRATexture(Graphics::Texture* texture)
		{
			m_MRAMap = texture;
		}

		void Material::Release()
		{
			GraphicsSystem::GetInstance()->DeleteMaterialBuffer(m_MaterialBuffer);
		}
	}
}