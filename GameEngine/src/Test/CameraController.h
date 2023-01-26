#pragma once

#include "GameEngine/Core/Component/Component.h"

using namespace GameEngine;
using namespace GameEngine::Core;

class CameraController : public Component
{
public:
	CameraController(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("CameraController"));
	~CameraController() override;

	void Awake() override;
	void Update() override;

private:
	std::shared_ptr<Transform> m_Transform;

	float m_Speed = 10.f;
};