#pragma once

#if defined(_MSC_VER) && defined(JSON_EXPORT)
#ifndef JSON_DLL_DECLSPEC
#define JSON_DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef JSON_DLL_DECLSPEC
#define JSON_DLL_DECLSPEC dll_import
#endif
#endif