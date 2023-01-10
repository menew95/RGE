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

struct Extend2D
{
	Extend2D()
		: _width(0)
		, _height(0)
	{}

	Extend2D(const Extend2D&) = default;

	inline Extend2D(uint32 width, uint32 height)
		: _width(width)
		, _height(height)
	{}

	uint32 _width;
	uint32 _height;
};

struct Extend3D
{
	Extend3D()
		: _width(0)
		, _height(0)
		, _depth(0)
	{}

	Extend3D(const Extend3D&) = default;

	inline Extend3D(uint32 width, uint32 height, uint32 depth)
		: _width(width)
		, _height(height)
		, _depth(depth)
	{}

	uint32 _width;
	uint32 _height;
	uint32 _depth;
};
