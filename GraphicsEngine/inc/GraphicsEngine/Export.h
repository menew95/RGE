#pragma once

#if defined(_MSC_VER) && defined(GRAPHICSENGINE_EXPORT)
#ifndef GRAPHICSENGINE_DLL_DECLSPEC
#define GRAPHICSENGINE_DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef GRAPHICSENGINE_DLL_DECLSPEC
#define GRAPHICSENGINE_DLL_DECLSPEC dll_import
#endif
#endif