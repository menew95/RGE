#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/System/Resources.h"
#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GameEngine/Core/Resource/Mesh.h"
#include "GameEngine/Core/Resource/Material.h"

#include "Importer/Importer.h"
#include "Importer/ImportInfo.h"


namespace GameEngine 
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(Resources, {}, {})

		void Resources::Init()
		{
			LoadFBX(TEXT("Asset/FBX/Wooden_Crate.fbx"));
			//LoadFBX(TEXT("Wooden_Crate.fbx"));
		}

		void* Resources::Load(const tstring& filePath)
		{
			// Todo : 리소스 로드를 템플릿 함수로 만들지 정해야함
			return nullptr;
		}

		void Resources::LoadFBX(const tstring& filePath)
		{
			using namespace Utility;

			PrefabData _newPrefabData;

			Importer _importer;

			_importer.LoadPrefabDataFormFile(filePath, _newPrefabData);

			for (auto& _pair : _newPrefabData._meshDataMap)
			{
				std::shared_ptr<Mesh> _newMesh = std::make_shared<Mesh>(_pair.second._vertexAttributes, _pair.second._indexAttributes);

				_newMesh->SetName(_pair.first);

				GraphicsSystem::GetInstance()->CreateMeshBuffer(_newMesh);
			}

			for (auto& _pair : _newPrefabData._materialDataMap)
			{
				std::shared_ptr<Material> _newMaterial = std::make_shared<Material>();

				_newMaterial->SetName(_pair.first);


			}
		}

		std::shared_ptr<GameEngine::Core::Mesh>& Resources::GetMesh(uuid uuid)
		{
			auto _find = m_MeshMap.find(uuid);

			if (_find != m_MeshMap.end())
			{
				return _find->second;
			}

			return m_NullMesh;
		}

		std::shared_ptr<GameEngine::Core::Material>& Resources::GetMaterial(uuid uuid)
		{
			auto _find = m_MaterialMap.find(uuid);

			if (_find != m_MaterialMap.end())
			{
				return _find->second;
			}

			return m_NullMaterial;
		}

	}
}