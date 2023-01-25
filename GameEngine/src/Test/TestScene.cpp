#include "GameEngine/GameEnginePCH.h"
#include "Test/TestScene.h"

#include "GameEngine/Core/GameObject.h"

#include "GameEngine/Core/Component/Camera.h"
#include "GameEngine/Core/Component/MeshFilter.h"
#include "GameEngine/Core/Component/MeshRenderer.h"

#include "GameEngine/Core/System/Resources.h"

TestScene::TestScene()
{
	std::shared_ptr<GameObject> _testObject = GameObject::Instantiate();

	auto _mesh = Resources::GetInstance()->GetMesh(TEXT("Plane.005"));
	//auto _mesh = Resources::GetInstance()->GetMesh(TEXT("Box"));
	auto _material = Resources::GetInstance()->GetMaterial(TEXT("Material.001"));

	_testObject->AddComponent<MeshFilter>()->SetSharedMesh(_mesh);
	_testObject->AddComponent<MeshRenderer>()->AddMaterial(_material);


	AddGameObject(_testObject);
}
