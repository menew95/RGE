#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/System/Resources.h"
#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GameEngine/Core/Resource/Mesh.h"
#include "GameEngine/Core/Resource/Material.h"

#include "Importer/Importer.h"
#include "Importer/ImportInfo.h"

#include "Struct/VertexAttribute.h"

namespace GameEngine 
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(Resources, {}, {})

		void Resources::Init()
		{
			LoadFBX(TEXT("Asset/FBX/Wooden_Crate.fbx"));




			{
				std::vector<Common::VertexAttribute> _vertexAttributes =
				{
					{ { -1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
				};

				std::vector<std::vector<uint32>> _indexAttributes = {
					{
						0, 1, 2,
						0, 2, 3,
						4, 5, 6,
						4, 6, 7,
						8, 9, 10,
						8, 10, 11,
						12, 13, 14,
						12, 14, 15,
						16, 17, 18,
						16, 18, 19,
						20, 21, 22,
						20, 22, 23
					}
				};
				std::shared_ptr<Mesh> _newMesh = std::make_shared<Mesh>(_vertexAttributes, _indexAttributes);

				_newMesh->SetName(TEXT("Box"));

				GraphicsSystem::GetInstance()->CreateMeshBuffer(_newMesh);

				m_MeshMap.insert(std::make_pair(TEXT("Box"), _newMesh));
			}
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

				m_MeshMap.insert(std::make_pair(_pair.first, _newMesh));
			}

			for (auto& _pair : _newPrefabData._materialDataMap)
			{
				std::shared_ptr<Material> _newMaterial = std::make_shared<Material>();

				_newMaterial->SetName(_pair.first);


				//_pair.second._albedoMapTexture;
				_pair.second._normalMapTexture;
				_pair.second._metalicRoughnessMapTexture;
				_pair.second._emissiveMapTexture;
				_pair.second._ambientMapTexture;
				_pair.second._specularMapTexture;

				GraphicsSystem::GetInstance()->CreateMaterialBuffer(_newMaterial);

				_newMaterial->SetAlbedoTexture(GraphicsSystem::GetInstance()->LoadTexture(_pair.second._albedoMapTexture));

				m_MaterialMap.insert(std::make_pair(_pair.first, _newMaterial));
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