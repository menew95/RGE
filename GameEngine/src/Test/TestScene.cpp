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

	{
		std::shared_ptr<GameObject> _pointLight = GameObject::Instantiate();

		_pointLight->SetName(TEXT("Point Light"));

		auto _lightCom = _pointLight->AddComponent<Light>();

		Vector3 _move = { 2, 1, 2 };

		_pointLight->GetTransform()->Translate(_move);

		_lightCom->SetLightType(2);

		_lightCom->SetRange(4.0f);

		AddGameObject(_pointLight);
	}

	{
		std::shared_ptr<GameObject> _pointLight = GameObject::Instantiate();

		_pointLight->SetName(TEXT("Point Light2"));

		auto _lightCom = _pointLight->AddComponent<Light>();

		Vector3 _move = { -2, 1, -2 };

		_pointLight->GetTransform()->Translate(_move);

		_lightCom->SetLightType(2);

		_lightCom->SetRange(5.0f);

		AddGameObject(_pointLight);
	}

	{
		std::shared_ptr<GameObject> _spotLight = GameObject::Instantiate();

		_spotLight->SetName(TEXT("Spot Light"));

		auto _lightCom = _spotLight->AddComponent<Light>();

		Vector3 _move = { -1, 2, 1 };

		_spotLight->GetTransform()->Translate(_move);

		_lightCom->SetLightType(0);

		_lightCom->SetRange(3.0f);

		_lightCom->SetLightColor();

		AddGameObject(_spotLight);
	}
}
