#pragma once

/*
 * Char and String
 */
#include <tchar.h>
#include <xstring>
#include <basetsd.h>

#ifndef __PLATFORM_INDEPENDENT_TYPE_DEFINED__
#define __PLATFORM_INDEPENDENT_TYPE_DEFINED__


using tchar = TCHAR;

using int8 = signed char;
using uint8 = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using int64 = int64_t;	// __int64
using uint64 = uint64_t;	// unsigned __int64

using uint = unsigned;
using byte = uint8;
using UID = uint64;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

using uuid = tstring;

using Layer = uint32;

#endif

struct Extent2D
{
	Extent2D()
		: _width(0)
		, _height(0)
	{}

	Extent2D(const Extent2D&) = default;

	inline Extent2D(uint32 width, uint32 height)
		: _width(width)
		, _height(height)
	{}

	uint32 _width;
	uint32 _height;
};

struct Extent3D
{
	Extent3D()
		: _width(0)
		, _height(0)
		, _depth(0)
	{}

	Extent3D(const Extent3D&) = default;

	inline Extent3D(uint32 width, uint32 height, uint32 depth)
		: _width(width)
		, _height(height)
		, _depth(depth)
	{}

	uint32 _width;
	uint32 _height;
	uint32 _depth;
};

struct Offset2D
{
	Offset2D() = default;
	Offset2D(const Offset2D&) = default;

	inline Offset2D(int32 x, int32 y) :
		x{ x },
		y{ y }
	{
	}

	int32 x = 0; //!< Offset X axis.
	int32 y = 0; //!< Offset Y axis.
};

struct Offset3D
{
	Offset3D() = default;
	Offset3D(const Offset3D&) = default;

	inline Offset3D(int32 x, int32 y, int32 z) :
		x{ x },
		y{ y },
		z{ z }
	{
	}

	int32 x = 0; //!< Offset X axis.
	int32 y = 0; //!< Offset Y axis.
	int32 z = 0; //!< Offset Z axis.
};