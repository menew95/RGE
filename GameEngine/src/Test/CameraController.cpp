#include "GameEngine/GameEnginePCH.h"
#include "Test/CameraController.h"

#include "GameEngine/Core/Component/Transform.h"

#include "GameEngine/Input.h"
#include "GameEngine/Time.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

CameraController::CameraController(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("CameraController")*/)
	:Component(gameObject, componentName)
{

}

CameraController::~CameraController()
{

}

void CameraController::Awake()
{
	m_Transform = GetComponent<Transform>();
}

void CameraController::Update()
{
	Math::Vector3 _rightDir = m_Transform->GetRight();
	Math::Vector3 _upDir = m_Transform->GetUp();
	Math::Vector3 _forwardDir = m_Transform->GetForward();
	Math::Quaternion _rotation = m_Transform->GetRotation();

	float _rotationPitch = 0;
	float _rotationY = 0;

	if (Input::GetKeyPress(EVirtualKey::MouseRight))
	{
		const auto _mouseDelta = Input::GetMouseMovementDelta();
		_rotationPitch = _mouseDelta.y * 0.1f * Deg2Rad;
		_rotationY = _mouseDelta.x * 0.1f * Deg2Rad;

		// // (deprecated) Matrix로 회전합니다.
		// if (_rotationPitch != 0)
		//     PitchUsingMatrix(_rotationPitch, _rightDir, _upDir, _forwardDir);
		// if (_rotationY != 0)
		//     RotateYUsingMatrix(_rotationY, _rightDir, _upDir, _forwardDir);

		// Quaternion으로 회전합니다.
		if (_rotationPitch != 0)
			_rotation = _rotation * Quaternion::CreateFromAxisAngle(_rightDir, _rotationPitch);
		if (_rotationY != 0)
			_rotation = _rotation * Quaternion::CreateFromAxisAngle(Vector3::Up, _rotationY);

		_rightDir = Vector3::Transform(Vector3::Right, _rotation);
		_upDir = Vector3::Transform(Vector3::Up, _rotation);
		_forwardDir = Vector3::Transform(Vector3::Forward, _rotation);
	}

	float _forward = 0;
	float _right = 0;
	float _up = 0;

	if (Input::GetKeyPress(EVirtualKey::Key_W))
		_forward += 1.f;
	if (Input::GetKeyPress(EVirtualKey::Key_S))
		_forward -= 1.f;

	if (Input::GetKeyPress(EVirtualKey::Key_D))
		_right += 1.f;
	if (Input::GetKeyPress(EVirtualKey::Key_A))
		_right -= 1.f;

	if (Input::GetKeyPress(EVirtualKey::Key_E))
		_up += 1.f;
	if (Input::GetKeyPress(EVirtualKey::Key_Q))
		_up -= 1.f;

	if (_forward != 0 || _right != 0 || _up != 0
		|| _rotationPitch != 0 || _rotationY != 0)
	{
		auto _direction = (_forwardDir * _forward + _rightDir * _right + _upDir * _up);
		_direction.Normalize();

		const auto _nextPosition = m_Transform->GetPosition() + _direction * m_Speed * static_cast<float>(Time::GetDeltaTime());

		m_Transform->SetWorldTM(Matrix{
			_rightDir.x, _rightDir.y, _rightDir.z, 0.f,
			_upDir.x, _upDir.y, _upDir.z, 0.f,
			_forwardDir.x, _forwardDir.y, _forwardDir.z, 0.f,
			_nextPosition.x, _nextPosition.y, _nextPosition.z, 1.f
			});

		//Log::Engine_Info("Cam Position : ({0}, {1}, {2])", (double)_nextPosition.x, (double)_nextPosition.y, (double)_nextPosition.z);
	}


	if (Input::GetKeyDown(EVirtualKey::Key_1))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._voxelGI = !_setting._voxelGI;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::Key_2))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._voxelDebug = !_setting._voxelDebug;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::Key_3))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._voxelDebugLine = !_setting._voxelDebugLine;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::Key_4))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._mode = 0;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::Key_5))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._mode = 1;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::Key_6))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._mode = 2;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::Key_7))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._mode = 3;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::Key_8))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._mode = 4;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyDown(EVirtualKey::OEM_Plus))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._exposure += 0.1f;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}
	else if (Input::GetKeyDown(EVirtualKey::OEM_Minus))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._exposure -= 0.1f;

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}

	if (Input::GetKeyPress(EVirtualKey::Key_Z))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._temp = std::clamp(_setting._temp + 0.01f, 0.01f, 1.f);

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}
	else if (Input::GetKeyPress(EVirtualKey::Key_X))
	{
		Graphics::RenderingSetting _setting = GraphicsSystem::GetInstance()->GetRenderingSetting();

		_setting._temp = std::clamp(_setting._temp - 0.01f, 0.01f, 1.f);

		GraphicsSystem::GetInstance()->SetRenderingSetting(_setting);
	}
}
