#include "GameEngine\GameEngineAPI.h"

#include "GameEngine\Core\Application.h"

#include "Common.h"

int APIENTRY _tWinMain(_In_ HINSTANCE _hInstance,
	_In_opt_ HINSTANCE _hPrevInstance,
	_In_ LPTSTR _lpCmdLine,
	_In_ int _nCmdShow
)
{
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