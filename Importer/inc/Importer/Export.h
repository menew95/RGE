#pragma once

#if defined(_MSC_VER) && defined(IMPORTER_EXPORT)
#ifndef IMPORTER_DLL_DECLSPEC
#define IMPORTER_DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef IMPORTER_DLL_DECLSPEC
#define IMPORTER_DLL_DECLSPEC dll_import
#endif
#endif