#pragma once

#include "Prefix.h"
#include <cmath>
#include <assert.h>

namespace RMath
{
	static const float gE = 2.71828182845904523536f;
	static const float gLog2E = 1.44269504088896340736f;
	static const float gLog10E = 0.434294481903251827651f;
	static const float gLn2 = 0.693147180559945309417f;
	static const float gLn10 = 2.30258509299404568402f;
	static const float gPI = 3.14159265358979323846f;
	static const float gHalfPI = 1.57079632679489661923f;
	static const float gQuarterPI = 0.785398163397448309616f;
	static const float gSqrt2 = 1.41421356237309504880f;
	static const float gReciprocalSqrt2 = 0.707106781186547524401f;
	static const float gSqrtHalf = 0.707106781186547524401f;

	inline float RSin(float radians) { return std::sin(radians); }
	inline float RCos(float radians) { return std::cos(radians); }
	inline float RTan(float radians) { return std::tan(radians); }
	inline float RASin(float x) { return std::asin(x); }
	inline float RACos(float x) { return std::acos(x); }
	inline float RATan(float x) { return std::atan(x); }
	inline float RATan2(float y, float x) { return std::atan2(y, x); }
	inline float RCotangent(float radians) { return 1.f / std::tan(radians); }
	inline float RFMod(float value, float modulo) { return std::fmod(value, modulo); }
	inline float RAbs(float value) { return std::abs(value); }
	inline float RFloor(float value) { return std::floor(value); }
	inline float RCeil(float value) { return std::ceil(value); }
	inline float RExp(float value) { return std::exp(value); }
	inline float RLog(float value) { return std::log(value); }

	template<typename T>
	inline T RSqrt(T value) { return std::sqrt(value); }

	template<typename T>
	inline T RRevetSqrt(T value) { return T(1) / std::sqrt(value); }

	template<typename T>
	inline bool RRevetSqrt_Checked(T* output, T value)
	{
		assert(output);

		if (value > T(-1e-15) && value < T(1e-15)) return false;
		*output = 1.f / std::sqrt(value);
		return true;
	}

	template<typename T>
	inline T RSqrt(T value) { return std::sqrt(value); }

	inline float Deg2Rad(float input) { return input / 180.f * gPI; }
	inline float Rad2Deg(float input) { return input * 180.f / gPI; }

	template < typename T >
	T Clamp(T value, T minval, T maxval) {
		return std::max(std::min(value, maxval), minval);
	}

	inline float Lerp(float lhs, float rhs, float alpha)
	{
		return (rhs - lhs) * alpha + lhs;
	}

	inline int Lerp(int lhs, int rhs, float alpha)
	{
		return int((rhs - lhs) * alpha + .5f) + lhs;
	}

	template<typename Type> const Type& Identity();
	template<typename Type> const Type& Zero();
}

using namespace RMath;