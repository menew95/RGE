#include "GameEngine/GameEnginePCH.h"
#include "Test/TestScene.h"

#include "GameEngine/Core/GameObject.h"

#include "GameEngine/Core/Component/Camera.h"
#include "GameEngine/Core/Component/Transform.h"
#include "GameEngine/Core/Component/MeshFilter.h"
#include "GameEngine/Core/Component/MeshRenderer.h"
#include "GameEngine/Core/Component/Light.h"

#include "GameEngine/Core/System/Resources.h"
#include "GameEngine/Core/Resource/Prefab.h"

#include "Test/CameraController.h"

TestScene::TestScene()
{
	auto camera = FindObject(TEXT("Camera"));

	camera->AddComponent<CameraController>();

	//std::shared_ptr<GameObject> _testObject = GameObject::Instantiate();
	//_testObject->SetName(TEXT("Box Object"));

	//auto _mesh = Resources::GetInstance()->GetMesh(TEXT("Plane.005"));
	////auto _mesh = Resources::GetInstance()->GetMesh(TEXT("Box"));
	//auto _material = Resources::GetInstance()->GetMaterial(TEXT("Material.001"));

	//_testObject->AddComponent<MeshFilter>()->SetSharedMesh(_mesh);
	//_testObject->AddComponent<MeshRenderer>()->AddMaterial(_material);

	//AddGameObject(_testObject);

	std::shared_ptr<GameObject> _wooden_CrateObject = Resources::GetInstance()->GetPrefab(TEXT("Asset/FBX/Wooden_Crate.fbx"))->GetRootGameObject();

	AddGameObjects(_wooden_CrateObject);

	std::shared_ptr<GameObject> _dirLight = GameObject::Instantiate();

	_dirLight->SetName(TEXT("Directional Light"));

	Math::Vector3 _eular = { 45, 45, 0};

	_dirLight->GetComponent<Transform>()->Rotate(_eular);

	auto _lightCom = _dirLight->AddComponent<Light>();


	AddGameObject(_dirLight);
}
