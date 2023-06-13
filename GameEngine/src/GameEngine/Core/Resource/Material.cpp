#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Resource\Material.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/RenderObject.h"

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

			m_MaterialBuffer->SetResource(reinterpret_cast<Graphics::Resource*>(texture), Graphics::ResourceType::Texture, 0);
		}

		void Material::SetNormalTexture(Graphics::Texture* texture)
		{
			m_NormalMap = texture;

			m_MaterialBuffer->SetResource(reinterpret_cast<Graphics::Resource*>(texture), Graphics::ResourceType::Texture, 1);
		}

		void Material::SetMRATexture(Graphics::Texture* texture)
		{
			m_MRAMap = texture;

			m_MaterialBuffer->SetResource(reinterpret_cast<Graphics::Resource*>(texture), Graphics::ResourceType::Texture, 2);
		}

		void Material::SetBuffer(Graphics::Resource* resource)
		{
			m_MaterialBuffer->SetBufferData(&m_StandardResource, resource);
		}

		void Material::Release()
		{
			GraphicsSystem::GetInstance()->ReleaseMaterialBuffer(m_MaterialBuffer);
		}
	}
}