#pragma once

#if defined(_MSC_VER) && defined(GRAPHICS_EXPORT)
#ifndef GRAPHICS_DLL_DECLSPEC
#define GRAPHICS_DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef GRAPHICS_DLL_DECLSPEC
#define GRAPHICS_DLL_DECLSPEC dll_import
#endif
#endif