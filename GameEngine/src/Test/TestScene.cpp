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
#include "Test/CubeMove.h"

TestScene::TestScene()
{
	auto camera = FindObject(TEXT("Camera"));

	camera->AddComponent<CameraController>();

	Vector3 _move = { 0, 0, -2 };

	camera->GetTransform()->Translate(_move);

	{
		std::shared_ptr<GameObject> _wooden_CrateObject_Base = Resources::GetInstance()->GetResource<Prefab>(TEXT("Wooden_Crate"))->GetRootGameObject();

		//AddGameObjects(_wooden_CrateObject_Base);

		Vector3 _move = { 0, 0, 10 };

		_wooden_CrateObject_Base->GetTransform()->Translate(_move);
	}

	{
		std::shared_ptr<GameObject> _wooden_CrateObject_Bump_MRA = Resources::GetInstance()->GetResource<Prefab>(TEXT("Wooden_Crate_Normal_MRA"))->GetRootGameObject();

		//AddGameObjects(_wooden_CrateObject_Bump_MRA);

		Vector3 _move = { -10, 0, 0 };

		_wooden_CrateObject_Bump_MRA->GetTransform()->Translate(_move);
	}

	{
		std::shared_ptr<GameObject> _joyObject = Resources::GetInstance()->GetResource<Prefab>(TEXT("Joy"))->GetRootGameObject();

		//AddGameObjects(_joyObject);

		Vector3 _move = { 10, 0, 0 };

		_joyObject->GetTransform()->Translate(_move);
	}

	{
		std::shared_ptr<GameObject> _labObject = Resources::GetInstance()->GetResource<Prefab>(TEXT("lab_12"))->GetRootGameObject();

		AddGameObjects(_labObject);

		Vector3 _move = { -3, -2, -2 };

		_labObject->GetTransform()->Translate(_move);
	}

	{
		std::shared_ptr<GameObject> _dirLight = GameObject::Instantiate();

		_dirLight->SetName(TEXT("Directional Light"));

		Math::Vector3 _eular = { 45, 45, 0 };

		_dirLight->GetComponent<Transform>()->Rotate(_eular);

		auto _lightCom = _dirLight->AddComponent<Light>();

		AddGameObject(_dirLight);
	}
#pragma region
	//{
	//	std::shared_ptr<GameObject> _pointLight = GameObject::Instantiate();

	//	_pointLight->SetName(TEXT("Point Light"));

	//	auto _lightCom = _pointLight->AddComponent<Light>();

	//	Vector3 _move = { 3, 1, 3 };

	//	_pointLight->GetTransform()->Translate(_move);

	//	_lightCom->SetLightType(2);

	//	_lightCom->SetRange(5.0f);

	//	_lightCom->SetLightColor(Math::Color::Red);

	//	AddGameObject(_pointLight);
	//}

	//{
	//	std::shared_ptr<GameObject> _pointLight = GameObject::Instantiate();

	//	_pointLight->SetName(TEXT("Point Light2"));

	//	auto _lightCom = _pointLight->AddComponent<Light>();

	//	Vector3 _move = { -3, 1, -1 };

	//	_pointLight->GetTransform()->Translate(_move);

	//	_lightCom->SetLightType(2);

	//	_lightCom->SetRange(5.0f);

	//	_lightCom->SetLightColor(Math::Color::Green);

	//	AddGameObject(_pointLight);
	//}

	//{
	//	std::shared_ptr<GameObject> _spotLight = GameObject::Instantiate();

	//	_spotLight->SetName(TEXT("Spot Light"));

	//	auto _lightCom = _spotLight->AddComponent<Light>();

	//	Vector3 _move = { -1, 2, 4 };

	//	_spotLight->GetTransform()->Translate(_move);


	//	Vector3 _rot = { 90, 0, 0 };
	//	_spotLight->GetTransform()->Rotate(_rot);

	//	_lightCom->SetLightType(0);

	//	//_lightCom->SetSpotAnlge(15.0f);

	//	_lightCom->SetLightColor(Math::Color::Blue);

	//	AddGameObject(_spotLight);
	//}
#pragma endregion TestLight

	{
		std::shared_ptr<GameObject> _spotLight = GameObject::Instantiate();

		_spotLight->SetName(TEXT("Spot Light"));

		auto _lightCom = _spotLight->AddComponent<Light>();

		Vector3 _move = { 2, 2.5, 1 };

		_spotLight->GetTransform()->Translate(_move);
		
		Vector3 _eular = { 90, 0, 60 };
		_spotLight->GetTransform()->Rotate(_eular);

		_lightCom->SetLightType(0);

		_lightCom->SetRange(10.0f);

		_lightCom->SetLightColor(Math::Color::White);

		AddGameObject(_spotLight);
	}

	{
		std::shared_ptr<GameObject> _colorCube = GameObject::Instantiate();

		AddGameObjects(_colorCube);

		Vector3 _move = { 0.2f, 0.2f, 0.2f };

		_colorCube->GetTransform()->Scale(_move);

		auto _filter = _colorCube->AddComponent<MeshFilter>();
		_colorCube->AddComponent<CubeMove>();
		auto _cube = Resources::GetInstance()->GetResource<Mesh>(TEXT("Box"));

		_filter->SetSharedMesh(_cube);

		auto _ren = _colorCube->AddComponent<MeshRenderer>();

		auto _red = Resources::GetInstance()->GetResource<Material>(TEXT("Red"));

		_ren->AddMaterial(_red);
	}

	{
		std::shared_ptr<GameObject> _colorCube = GameObject::Instantiate();

		AddGameObjects(_colorCube);

		Vector3 _move2 = { 0, -1, 0 };
		Vector3 _scale = { 0.2f, 0.2f, 0.2f };

		_colorCube->GetTransform()->Translate(_move2);
		_colorCube->GetTransform()->Scale(_scale);

		auto _filter = _colorCube->AddComponent<MeshFilter>();
		_colorCube->AddComponent<CubeMove>();
		auto _cube = Resources::GetInstance()->GetResource<Mesh>(TEXT("Box"));

		_filter->SetSharedMesh(_cube);

		auto _ren = _colorCube->AddComponent<MeshRenderer>();

		auto _red = Resources::GetInstance()->GetResource<Material>(TEXT("Green"));

		_ren->AddMaterial(_red);
	}
}
