#pragma once

#include "DebugHelper.h"

#include <Windows.h>
#include <debugapi.h>

#include "StringHelper.h"

template <typename ... Args>
void DebugHelper::PrintDebugString(const tstring& _format, Args ... _args)
{
    const tstring _formatted = StringHelper::Format(&_format, _args ...);
    OutputDebugString(_formatted.c_str());
}

template <typename ... Args>
void DebugHelper::PrintDebugString(tstring& _format, Args... _args)
{
    const tstring _formatted = StringHelper::Format(&_format, _args ...);
    OutputDebugString(_formatted.c_str());
}

template <typename ... Args>
void DebugHelper::PrintDebugString(const TCHAR* _format, Args ... _args)
{
    const tstring _formatted = StringHelper::Format(_format, _args ...);
    OutputDebugString(_formatted.c_str());
}

template <typename ... Args>
void DebugHelper::PrintDebugString(TCHAR* _format, Args... _args)
{
    const tstring _formatted = StringHelper::Format(_format, _args ...);
    OutputDebugString(_formatted.c_str());
}
