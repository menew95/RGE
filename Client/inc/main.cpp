#include <Windows.h>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <shellapi.h>
#include <cstdio>

#include "GameEngine\GameEngineAPI.h"

#include "GameEngine\Core\Application.h"

#include "Common.h"

#if defined(DEBUG) | defined(_DEBUG)

#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)

#if defined(DEBUG_ENABLE_VLD)

#include <vld.h>

#if defined(_WIN64)
#pragma comment(lib, "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win64\\vld.lib")
#elif
#pragma comment(lib, "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win32\\vld.lib")
#endif

#endif

#endif
void RedirectIOToConsole()
{
	// maximum mumber of lines the output console should have
	static const WORD MAX_CONSOLE_LINES = 500;

	int hConHandle;
	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE* fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	coninfo.dwSize.Y = MAX_CONSOLE_LINES;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console

	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "r");

	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
	// point to console as well
	std::ios::sync_with_stdio();
}

int APIENTRY _tWinMain(_In_ HINSTANCE _hInstance,
	_In_opt_ HINSTANCE _hPrevInstance,
	_In_ LPTSTR _lpCmdLine,
	_In_ int _nCmdShow
)
{
	RedirectIOToConsole();

	using namespace GameEngine;

#ifdef _DEBUG
#ifdef x64
	auto _gameEngineModule = LoadLibrary(_T("GameEngine_Debug_x64.dll"));
#else
	auto _gameEngineModule = LoadLibrary(_T("GameEngine_Debug_WIN32.dll"));
#endif // x64
#else
#ifdef x64
	auto _gameEngineModule = LoadLibrary(_T("GameEngine_Release_x64.dll"));
#else
	auto _gameEngineModule = LoadLibrary(_T("GameEngine_Release_WIN32.dll"));
#endif // x64
#endif
	assert(_gameEngineModule != NULL);

	auto _applicationConstructor = (ApplicationConstructor)GetProcAddress(_gameEngineModule, "CreateApplication");

	assert(_applicationConstructor != NULL);

	auto _applicationDestructor = (ApplicationDestructor)GetProcAddress(_gameEngineModule, "DeleteApplication");

	assert(_applicationDestructor != NULL);

	auto* _application = _applicationConstructor();


	{
		if (_application->Initallize(true, _hInstance, TEXT("ClientWindow"), TEXT("Client"), 1280, 720) == false)
		{
			return -1;
		}

		while (true)
		{
			MSG _msg;

			if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&_msg);
				DispatchMessage(&_msg);

				if (_msg.message == WM_QUIT)
					break;
			}
			else
			{
				if (_application->Update() == true)
					break;
			}
		}

		_application->Quit();
	}

	_applicationDestructor(_application);

	FreeLibrary(_gameEngineModule);

	return 0;
}