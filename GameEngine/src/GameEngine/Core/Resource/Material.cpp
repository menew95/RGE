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

			/*Graphics::UpdateResourceData _data;

			_data._resourceType = Graphics::ResourceType::Texture;

			_data._index = 4;

			_data._dataSrc = texture;

			m_MaterialBuffer->GetUpdateReosurceData().push_back(_data);*/
		}

		void Material::SetNormalTexture(Graphics::Texture* texture)
		{
			m_NormalMap = texture;

			/*Graphics::UpdateResourceData _data;

			_data._resourceType = Graphics::ResourceType::Texture;

			_data._index = 5;

			_data._dataSrc = texture;

			m_MaterialBuffer->GetUpdateReosurceData().push_back(_data);*/
		}

		void Material::SetMRATexture(Graphics::Texture* texture)
		{
			m_MRAMap = texture;

			/*Graphics::UpdateResourceData _data;

			_data._resourceType = Graphics::ResourceType::Texture;

			_data._index = 6;

			_data._dataSrc = texture;

			m_MaterialBuffer->GetUpdateReosurceData().push_back(_data);*/
		}

		void Material::Release()
		{
			GraphicsSystem::GetInstance()->DeleteMaterialBuffer(m_MaterialBuffer);
		}
	}
}