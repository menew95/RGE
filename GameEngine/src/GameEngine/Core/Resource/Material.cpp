#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Resource\Material.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

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

		void Material::Release()
		{
			GraphicsSystem::GetInstance()->DeleteMaterialBuffer(m_MaterialBuffer);
		}
	}
}