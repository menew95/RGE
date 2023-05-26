#pragma once

#include "GameEngine/Core/Component/Component.h"

using namespace GameEngine;
using namespace GameEngine::Core;

class CubeMove : public Component
{
public:
	CubeMove(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("CubeMove"));
	~CubeMove() override;

	void Awake() override;
	void Update() override;

private:
	std::shared_ptr<Transform> m_Transform;

	float m_Speed = 0.5f;

	std::vector<Vector3> m_PatrolPos;
	uint32 _currIdx = 0;
};