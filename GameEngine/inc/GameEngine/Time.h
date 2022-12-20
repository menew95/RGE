#pragma once

#include "Common.h"

#include <chrono>

namespace GameEngine
{
	class GAME_ENGINE_API Time
	{
		DECLARE_SINGLETON_CLASS(Time)
    public:
        void Initialize();

        void PreTick();

    private:
        std::chrono::system_clock::time_point m_StartGameTime;

        std::chrono::system_clock::time_point m_LastPreTickTime;

        std::chrono::system_clock::time_point m_CurrentPreTickTime;

    private:
        long long m_GameTimeNanos;

        long long m_DeltaTimeNanos;

        long long m_ElapsedDelayedTimeNanos;

    public:
        inline static double GetGameTimeMillis() { return Instance.m_GameTimeNanos * NANOS_TO_MILLIS; }

        inline static double GetDeltaTimeMillis() { return Instance.m_DeltaTimeNanos * NANOS_TO_MILLIS; }

        inline static double GetGameTime() { return Instance.m_GameTimeNanos * NANOS_TO_SECOND; }

        inline static double GetDeltaTime() { return Instance.m_DeltaTimeNanos * NANOS_TO_SECOND; }

        inline static auto GetGameTimePoint() { return Instance.m_CurrentPreTickTime; }

    public:
        static constexpr long long SECOND_TO_MILLIS = 1'000;
        static constexpr long long SECOND_TO_MICROS = 1'000'000;
        static constexpr long long SECOND_TO_NANOS = 1'000'000'000;
        static constexpr double MILLIS_TO_SECOND = 1. / 1'000.;
        static constexpr long long MILLIS_TO_MICROS = 1'000;
        static constexpr long long MILLIS_TO_NANOS = 1'000'000;
        static constexpr double MICROS_TO_SECOND = 1. / 1'000'000.;
        static constexpr double MICROS_TO_MILLIS = 1. / 1'000.;
        static constexpr long long MICROS_TO_NANOS = 1'000;
        static constexpr double NANOS_TO_SECOND = 1. / 1'000'000'000.;
        static constexpr double NANOS_TO_MILLIS = 1. / 1'000'000.;
        static constexpr double NANOS_TO_MICROS = 1. / 1'000.;
	};
}