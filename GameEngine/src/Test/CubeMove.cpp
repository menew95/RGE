#include "GameEngine/GameEnginePCH.h"
#include "Test/CubeMove.h"

#include "GameEngine/Core/Component/Transform.h"

#include "GameEngine/Input.h"
#include "GameEngine/Time.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

CubeMove::CubeMove(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("CubeMove")*/)
	: Component(gameObject, componentName)
{

}

CubeMove::~CubeMove()
{

}

void CubeMove::Awake()
{
	m_Transform = GetComponent<Transform>();

	auto _pos = m_Transform->GetPosition();

	m_PatrolPos.push_back(_pos);
	m_PatrolPos.push_back(_pos + Vector3(3.f, 0.f, 0.f));
	m_PatrolPos.push_back(_pos + Vector3(3.f, 0.f, 3.f));
	m_PatrolPos.push_back(_pos + Vector3(0.f, 0.f, 3.0f));
}

void CubeMove::Update()
{
	auto _currPos = m_Transform->GetPosition();

	if (Vector3::Distance(_currPos, m_PatrolPos[_currIdx]) < 0.001)
	{
		_currIdx++;
		if (_currIdx == m_PatrolPos.size())
			_currIdx = 0;
	}

	auto _dir = m_PatrolPos[_currIdx] - _currPos;

	_dir.Normalize();

	auto _nextPosition = _currPos + _dir * m_Speed * static_cast<float>(Time::GetDeltaTime());

	m_Transform->Translate(_nextPosition);
}
