// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef GraphicsModulePCH_H
#define GraphicsModulePCH_H

#include "Common.h"

#ifdef GRAPHICS_EXPORT
#ifndef GRAPHICS_DLL_DECLSPEC
#define GRAPHICS_DLL_DECLSPEC dll_export
#endif
#else
#ifndef GRAPHICS_DLL_DECLSPEC
#define GRAPHICS_DLL_DECLSPEC dll_import
#endif
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x, msg)									\
	{									\
		HRESULT hr = (x);						\
		if(FAILED(hr))							\
		{								\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
				NULL,						\
				hr,						\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,				\
				0,						\
				NULL);					        \
			MessageBox(NULL, output, TEXT(msg), MB_OK);		\
		}								\
	}
#endif
#else
#ifndef HR
#define HR(x, msg) (x)
#endif
#endif 

#endif //PCH_H
