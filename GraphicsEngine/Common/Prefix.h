#pragma once


#define dll_export      __declspec(dllexport)
#define dll_import      __declspec(dllimport)

#if _MSC_VER <= 1600
#define thread_local    __declspec(thread)
#endif

#if !defined(dimof)
#define dimof(x) (sizeof(x)/sizeof(*x))
#endif

#pragma warning(disable:4481)   //  warning C4481: nonstandard extension used: override specifier 'override'

static const unsigned MaxPath = 260;    /* max. length of full pathname */
static const unsigned MaxDrive = 3;    /* max. length of drive component */
static const unsigned MaxDir = 256;    /* max. length of path component */
static const unsigned MaxFilename = 256;    /* max. length of file name component */
static const unsigned MaxExtension = 256;    /* max. length of extension component */


typedef void* RHandle;

#if defined(_DEBUG)
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW

#endif

#if !defined(foreach)
#define foreach(iteratorName, ContainerType, container)                                         \
        for (auto iteratorName=container.begin(); iteratorName!=container.end(); ++iteratorName)    \
        /**/
#endif

#if !defined(foreach_const)
#define foreach_const(iteratorName, ContainerType, container)                                   \
        for (auto iteratorName=container.cbegin(); iteratorName!=container.cend(); ++iteratorName)  \
        /**/
#endif

template<bool B> struct constant_expression { static bool result() { return true; } };
template<> struct constant_expression<false> { static bool result() { return false; } };