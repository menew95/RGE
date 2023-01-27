#include "GameEngine/GameEnginePCH.h"
#include "Test/TestScene.h"

#include "GameEngine/Core/GameObject.h"

#include "GameEngine/Core/Component/Camera.h"
#include "GameEngine/Core/Component/Transform.h"
#include "GameEngine/Core/Component/MeshFilter.h"
#include "GameEngine/Core/Component/MeshRenderer.h"
#include "GameEngine/Core/Component/Light.h"

#include "GameEngine/Core/System/Resources.h"

#include "Test/CameraController.h"

TestScene::TestScene()
{
	std::shared_ptr<GameObject> _testObject = GameObject::Instantiate();
	_testObject->SetName(TEXT("Box Object"));

	auto _mesh = Resources::GetInstance()->GetMesh(TEXT("Plane.005"));
	//auto _mesh = Resources::GetInstance()->GetMesh(TEXT("Box"));
	auto _material = Resources::GetInstance()->GetMaterial(TEXT("Material.001"));

	_testObject->AddComponent<MeshFilter>()->SetSharedMesh(_mesh);
	_testObject->AddComponent<MeshRenderer>()->AddMaterial(_material);

	auto camera = FindObject(TEXT("Camera"));

	camera->AddComponent<CameraController>();

	AddGameObject(_testObject);

	std::shared_ptr<GameObject> _dirLight = GameObject::Instantiate();

	_dirLight->SetName(TEXT("Directional Light"));

	Math::Vector3 _eular = { 0, 45, -45};

	_dirLight->GetComponent<Transform>()->Rotate(_eular);

	auto _lightCom = _dirLight->AddComponent<Light>();


	AddGameObject(_dirLight);
}
