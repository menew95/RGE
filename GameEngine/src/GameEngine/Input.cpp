#include "GameEngine\GameEnginePCH.h"

namespace GameEngine
{
	Input* Input::Instance = nullptr;

	Input::Input()
	{
		assert(Instance == nullptr);
		Instance = this;
	}

	Input::~Input()
	{
		assert(Instance != nullptr);
		Instance = nullptr;
	}

	void Input::Initialize(HWND _hWnd)
	{
		m_hWnd = _hWnd;

		for (int i = 0; i < VKEY_COUNT; ++i)
		{
			m_KeyInfos[i] = { EKeyState::None, std::chrono::system_clock::time_point() };
			m_LastKeyMessageDowns[i] = false;
		}

		m_CurrentMousePosition = Vector2::Zero;
		m_PreviousMousePosition = Vector2::Zero;
		m_MouseMovementDelta = Vector2::Zero;
		m_LastMouseDragStartPosition = Vector2::Zero;
		m_LastMouseDragEndPosition = Vector2::Zero;
		m_bMouseMove = false;
	}

	void Input::PreTick()
	{
		BOOL _bSuccess;

		POINT _currentMousePoint;
		_bSuccess = GetCursorPos(&_currentMousePoint);
		_bSuccess = ScreenToClient(m_hWnd, &_currentMousePoint);
		const Vector2 _newMousePosition{ static_cast<float>(_currentMousePoint.x), static_cast<float>(_currentMousePoint.y) };

		// ���콺 ��ġ ������ �����մϴ�.
		m_PreviousMousePosition = m_CurrentMousePosition;
		m_CurrentMousePosition = _newMousePosition;
		m_MouseMovementDelta = m_CurrentMousePosition - m_PreviousMousePosition;

		// ���콺 �巡�װ� ���۵ǰ� ������ ��ġ ������ �����մϴ�.
		if (GetKeyState(EVirtualKey::MouseLeft) == EKeyState::Down)
		{
			m_LastMouseDragStartPosition = m_CurrentMousePosition;
			m_LastMouseDragEndPosition.x = m_LastMouseDragEndPosition.y = 0;
		}
		else if (GetKeyState(EVirtualKey::MouseLeft) == EKeyState::Up)
		{
			m_LastMouseDragEndPosition = m_CurrentMousePosition;
		}

		// ���콺�� ������������ ���� ���θ� �����մϴ�.
		m_bMouseMove = m_CurrentMousePosition != m_PreviousMousePosition;

		// ���콺 �� delta�� �����մϴ�.
		int _wheelDelta = 0;
		MouseWheelMessageInfo _info;
		while (PopMouseWheelMessage(_info))
		{
			_wheelDelta += _info.wheelDelta;
		}
		m_MouseWheelDelta = _wheelDelta;
	}

	EKeyState Input::GetKeyState(EVirtualKey _vKey)
	{
		const auto _vKeyIndex = static_cast<int>(_vKey);
		const auto _keyInfo = Instance->m_KeyInfos[_vKeyIndex];
		const auto _gameTimePoint = Time::GetGameTimePoint();

		// �̹� �����ӿ� �̹� �����Ǿ����� �˻��մϴ�.
		// �̹� �����Ǿ��ٸ�, ���� ���������� ������ ���� �״�� ��ȯ�մϴ�.
		if (_keyInfo.lastUpdatedTime >= _gameTimePoint)
			return _keyInfo.state;

		// �̹� �����ӿ��� ó������ Ű�� ���°� ��û�� ���̹Ƿ�
		// Ű�� ���� ������ �����մϴ�.

		const auto _currentKeyState = GetAsyncKeyState(_vKeyIndex);

		const auto _elapsedTimeMillis
			= std::chrono::duration_cast<std::chrono::milliseconds>(
				_gameTimePoint - _keyInfo.lastUpdatedTime).count();

		if (_elapsedTimeMillis > INPUT_TRACE_THRESHOLD_TIME
			&& _currentKeyState & 0x8000)
		{
			// �����Ϸ��� Ű�� ���������� ���ŵ� �ð��� �����Ǿ��ٸ�, _previousKeyPress���� ���� �����ӿ� ���ŵ� ���̶�� ���� �� �����ϴ�.
			// �׷��ٸ� Ű�� ���¸� �Ǻ��ϱⰡ ��������ϴ�.
			// ���� �̷� ���, Ű �Է��� �� ������ ������ �� �����մϴ�.

			return EKeyState::None;
		}

		const auto _previousKeyPress =
			static_cast<int>(_keyInfo.state)
			& 0x0001;

		if (_currentKeyState == 0x0000)
		{
			// Ű�� ������ �ʾ��� ��, ������ GetAsyncKeyState�� ȣ���� ���� �����Ͽ�
			// ������ ���°� true�� �� Up, (�� ���� ���� �幮 ����Դϴ�)
			// false��� �� �ð����� ���� ���� ���� ���ٴ� ���̹Ƿ� None���� �Ǻ��մϴ�.

			Instance->m_KeyInfos[_vKeyIndex].state
				= _previousKeyPress ? EKeyState::Up : EKeyState::None;
		}
		else if (_currentKeyState == 0x0001)
		{
			// Ű�� �������� �ʾ�����, ������ GetAsyncKeyState�� ȣ���� ���Ŀ� �Է��� ������ �ִٸ�
			// ������ ���°� true�� �� Up,
			// false��� Ű�� ���� ������ ������ �´ٴ� ������ �����Ͽ� DownAndUp���� �Ǻ��մϴ�.

			Instance->m_KeyInfos[_vKeyIndex].state
				= _previousKeyPress ? EKeyState::Up : EKeyState::DownAndUp;
		}
		else if (_currentKeyState & 0x8000)
		{
			// Ű�� �����ִٸ�, ���� ���¸� �ݿ��ؼ� Down �Ǵ� Hold�� �Ǻ��մϴ�.

			Instance->m_KeyInfos[_vKeyIndex].state
				= _previousKeyPress ? EKeyState::Hold : EKeyState::Down;
		}
		else
		{
			Instance->m_KeyInfos[_vKeyIndex].state = EKeyState::None;
		}

		// ���� ���������� ������ �ð���
		// ���� �������� �ð����� �����մϴ�.
		Instance->m_KeyInfos[_vKeyIndex].lastUpdatedTime = _gameTimePoint;

		return Instance->m_KeyInfos[_vKeyIndex].state;
	}

	bool Input::GetKeyPress(EVirtualKey _vKey)
	{
		const auto _keyState = GetKeyState(_vKey);
		return _keyState & EKeyStateBit::Bit_Current;
	}

	bool Input::GetKeyDown(EVirtualKey _vKey)
	{
		const auto _keyState = GetKeyState(_vKey);
		return _keyState & EKeyStateBit::Bit_Down;
	}

	bool Input::GetKeyUp(EVirtualKey _vKey)
	{
		const auto _keyState = GetKeyState(_vKey);
		return _keyState & EKeyStateBit::Bit_Up;
	}

	int Input::GetAxisHorizontal()
	{
		int _axis = 0;

		if (GetKeyPress(EVirtualKey::RightArrow))
			_axis += 1;

		if (GetKeyPress(EVirtualKey::LeftArrow))
			_axis -= 1;

		return _axis;
	}

	int Input::GetAxisVertical()
	{
		int _axis = 0;

		if (GetKeyPress(EVirtualKey::UpArrow))
			_axis -= 1;

		if (GetKeyPress(EVirtualKey::DownArrow))
			_axis += 1;

		return _axis;
	}
}
