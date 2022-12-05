#include "GameEngine\GameEnginePCH.h"

namespace GameEngine
{
	DEFINE_SINGLETON_CLASS(Input, {}, {})

	void Input::Initialize(HWND _hWnd)
	{
		m_hWnd = _hWnd;

		for (int i = 0; i < VKEY_COUNT; ++i)
		{
			m_KeyInfos[i] = { EKeyState::None, std::chrono::system_clock::time_point() };
			m_LastKeyMessageDowns[i] = false;
		}

		m_CurrentMousePosition = Math::Vector2::Zero;
		m_PreviousMousePosition = Math::Vector2::Zero;
		m_MouseMovementDelta = Math::Vector2::Zero;
		m_LastMouseDragStartPosition = Math::Vector2::Zero;
		m_LastMouseDragEndPosition = Math::Vector2::Zero;
		m_bMouseMove = false;
	}

	void Input::PreTick()
	{
		BOOL _bSuccess;

		POINT _currentMousePoint;
		_bSuccess = GetCursorPos(&_currentMousePoint);
		_bSuccess = ScreenToClient(m_hWnd, &_currentMousePoint);
		const Math::Vector2 _newMousePosition{ static_cast<float>(_currentMousePoint.x), static_cast<float>(_currentMousePoint.y) };

		// 마우스 위치 정보를 갱신합니다.
		m_PreviousMousePosition = m_CurrentMousePosition;
		m_CurrentMousePosition = _newMousePosition;
		m_MouseMovementDelta = m_CurrentMousePosition - m_PreviousMousePosition;

		// 마우스 드래그가 시작되고 끝나는 위치 정보를 갱신합니다.
		if (GetKeyState(EVirtualKey::MouseLeft) == EKeyState::Down)
		{
			m_LastMouseDragStartPosition = m_CurrentMousePosition;
			m_LastMouseDragEndPosition.x = m_LastMouseDragEndPosition.y = 0;
		}
		else if (GetKeyState(EVirtualKey::MouseLeft) == EKeyState::Up)
		{
			m_LastMouseDragEndPosition = m_CurrentMousePosition;
		}

		// 마우스를 움직였는지에 대한 여부를 갱신합니다.
		m_bMouseMove = m_CurrentMousePosition != m_PreviousMousePosition;
	}

	EKeyState Input::GetKeyState(EVirtualKey _vKey)
	{
		const auto _vKeyIndex = static_cast<int>(_vKey);
		const auto _keyInfo = m_KeyInfos[_vKeyIndex];
		const auto _gameTimePoint = Time::Instance.GetGameTimePoint();

		// 이번 프레임에 이미 추적되었는지 검사합니다.
		// 이미 추적되었다면, 가장 마지막으로 갱신한 값을 그대로 반환합니다.
		if (_keyInfo.lastUpdatedTime >= _gameTimePoint)
			return _keyInfo.state;

		// 이번 프레임에서 처음으로 키의 상태가 요청된 것이므로
		// 키의 상태 정보를 갱신합니다.

		const auto _currentKeyState = GetAsyncKeyState(_vKeyIndex);

		const auto _elapsedTimeMillis
			= std::chrono::duration_cast<std::chrono::milliseconds>(
				_gameTimePoint - _keyInfo.lastUpdatedTime).count();

		if (_elapsedTimeMillis > INPUT_TRACE_THRESHOLD_TIME
			&& _currentKeyState & 0x8000)
		{
			// 추적하려는 키가 마지막으로 갱신된 시간이 오래되었다면, _previousKeyPress값이 지난 프레임에 갱신된 값이라고 믿을 수 없습니다.
			// 그렇다면 키의 상태를 판별하기가 어려워집니다.
			// 따라서 이럴 경우, 키 입력을 뗄 때까지 무시한 뒤 갱신합니다.

			return EKeyState::None;
		}

		const auto _previousKeyPress =
			static_cast<int>(_keyInfo.state)
			& 0x0001;

		if (_currentKeyState == 0x0000)
		{
			// 키를 누르지 않았을 때, 지난번 GetAsyncKeyState를 호출한 값과 대조하여
			// 지난번 상태가 true일 때 Up, (이 경우는 극히 드문 경우입니다)
			// false라면 그 시간동안 누른 적이 전혀 없다는 것이므로 None으로 판별합니다.

			m_KeyInfos[_vKeyIndex].state
				= _previousKeyPress ? EKeyState::Up : EKeyState::None;
		}
		else if (_currentKeyState == 0x0001)
		{
			// 키가 눌려있지 않았지만, 지난번 GetAsyncKeyState를 호출한 이후에 입력의 흔적이 있다면
			// 지난번 상태가 true일 때 Up,
			// false라면 키를 아주 빠르게 눌렀다 뗏다는 것으로 간주하여 DownAndUp으로 판별합니다.

			m_KeyInfos[_vKeyIndex].state
				= _previousKeyPress ? EKeyState::Up : EKeyState::DownAndUp;
		}
		else if (_currentKeyState & 0x8000)
		{
			// 키가 눌려있다면, 지난 상태를 반영해서 Down 또는 Hold로 판별합니다.

			m_KeyInfos[_vKeyIndex].state
				= _previousKeyPress ? EKeyState::Hold : EKeyState::Down;
		}
		else
		{
			m_KeyInfos[_vKeyIndex].state = EKeyState::None;
		}

		// 가장 마지막으로 추적된 시간을
		// 현재 프레임의 시간으로 갱신합니다.
		m_KeyInfos[_vKeyIndex].lastUpdatedTime = _gameTimePoint;

		return m_KeyInfos[_vKeyIndex].state;
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