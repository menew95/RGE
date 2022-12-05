#include "GameEngine\GameEnginePCH.h"

namespace GameEngine
{
	DEFINE_SINGLETON_CLASS(Time, {}, {})

	void Time::Initialize()
	{
		m_StartGameTime = std::chrono::system_clock::now();
		m_LastPreTickTime = m_StartGameTime;
		m_CurrentPreTickTime = m_StartGameTime;
		m_GameTimeNanos = 0;
		m_DeltaTimeNanos = 0;
	}

	void Time::PreTick()
	{
		m_LastPreTickTime = m_CurrentPreTickTime;

		m_CurrentPreTickTime = std::chrono::system_clock::now();

		// DeltaTime�� ���մϴ�.
		m_DeltaTimeNanos =
			std::chrono::duration_cast<std::chrono::nanoseconds>(
				m_CurrentPreTickTime - m_LastPreTickTime).count();

		// TODO: ������� ���Ǹ� ����, �� �����Ӱ��� �ð� ������ �ſ� ����� ��, delta time�� �״�� ������� �ʰ� clamp�մϴ�.
		// �̸� 20ms ���� �����߽��ϴ�. ������ ������ ���� Ƚ���� �ּ� 50fps �̻����� ������ ������ �̻������ϴ�.
		constexpr long long CLAMP_MAX_DELTA_TIME_NANOS = SECOND_TO_NANOS / 50;
		constexpr long long CLAMP_MAX_DELTA_TIME_MILLIS = static_cast<long long>(
			CLAMP_MAX_DELTA_TIME_NANOS * NANOS_TO_MILLIS
			);

		if (m_DeltaTimeNanos > CLAMP_MAX_DELTA_TIME_NANOS)
		{
			m_DeltaTimeNanos = CLAMP_MAX_DELTA_TIME_MILLIS;

			const auto _startDelayTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				m_LastPreTickTime + std::chrono::nanoseconds(CLAMP_MAX_DELTA_TIME_NANOS)
				);

			const auto _delayedDuration = m_CurrentPreTickTime - _startDelayTime;

			m_ElapsedDelayedTimeNanos = m_ElapsedDelayedTimeNanos + _delayedDuration.count();
		}

		m_GameTimeNanos =
			std::chrono::duration_cast<std::chrono::nanoseconds>(m_CurrentPreTickTime - m_StartGameTime).count() - m_ElapsedDelayedTimeNanos;
	}
}