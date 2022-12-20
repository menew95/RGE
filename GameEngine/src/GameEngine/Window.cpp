#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Window.h"
#include "Common\StringHelper.h"

namespace GameEngine
{
	DEFINE_SINGLETON_CLASS(Window, {}, {})

	HRESULT Window::Initialize(bool showCmd, HINSTANCE hInstance, const tstring& windowClassName, const tstring& windowName, UINT width, UINT height)
	{
		m_WindowClassName = windowClassName;
		m_WindowName = windowName;
		m_WindowInfo._hInstance = hInstance;
		m_WindowInfo._width = width;
		m_WindowInfo._height = height;
		m_WindowClassNameW = StringHelper::ToWString(m_WindowClassName);

		HRESULT _hr = S_OK;

		_hr = RegisterWindow(hInstance);

		_hr = InitializeWindow(showCmd);
		assert(_hr == S_OK);
		
		return _hr;
	}

	ATOM Window::RegisterWindow(HINSTANCE hInstance)
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Window::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THANKSLIGHT));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = m_WindowClassNameW.c_str();
		//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		const ATOM _result = RegisterClassEx(&wcex);

		if (_result == 0)
		{
			auto _error = GetLastError();
			assert(false);
		}

		return _result;
		//return RegisterClassExW(&wcex);
	}

	HRESULT Window::InitializeWindow(bool showCmd)
	{
		// Create the window.
		m_WindowInfo._hWnd = CreateWindow(
			m_WindowClassName.c_str(),
			m_WindowName.c_str(),
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			static_cast<UINT>(m_WindowInfo._width),
			static_cast<UINT>(m_WindowInfo._height),
			NULL,
			NULL,
			m_WindowInfo._hInstance,
			this
		);

		assert(m_WindowInfo._hWnd != nullptr);

		ShowWindow(m_WindowInfo._hWnd, showCmd);

		if (UpdateWindow(m_WindowInfo._hWnd) == false)
			return S_FALSE;

		return S_OK;
	}

	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
				break;
			}
			case WM_MOUSEMOVE:
			{
				RECT clidentRC;
				POINT p1, p2;

				GetClientRect(hWnd, &clidentRC);

				p1.x = clidentRC.left;
				p1.y = clidentRC.top;
				p2.x = clidentRC.right;
				p2.y = clidentRC.bottom;

				ClientToScreen(hWnd, &p1);
				ClientToScreen(hWnd, &p2);

				clidentRC.left = p1.x;
				clidentRC.top = p1.y;
				clidentRC.right = p2.x;
				clidentRC.bottom = p2.y;

				float middleOfScrrenX = (clidentRC.left + clidentRC.right) / 2.0f;
				float middleOfScrrenY = (clidentRC.top + clidentRC.bottom) / 2.0f;

				POINT screenPos;
				screenPos.x = LOWORD(lParam);
				screenPos.y = HIWORD(lParam);

				ClientToScreen(hWnd, &screenPos);

				return 0;
			}
			case WM_SIZE:
			{
				//마우스 화면내 움직이기 고정
				{
					RECT clidentRC;
					POINT p1, p2;

					GetClientRect(hWnd, &clidentRC);

					p1.x = clidentRC.left;
					p1.y = clidentRC.top;
					p2.x = clidentRC.right;
					p2.y = clidentRC.bottom;

					ClientToScreen(hWnd, &p1);
					ClientToScreen(hWnd, &p2);

					clidentRC.left = p1.x;
					clidentRC.top = p1.y;
					clidentRC.right = p2.x;
					clidentRC.bottom = p2.y;

					ClipCursor(&clidentRC);

					Window::GetInstance()->GetWindowInfo()._width = LOWORD(lParam);
					Window::GetInstance()->GetWindowInfo()._height = HIWORD(lParam);
				}

				//// Save the new client area dimensions.
				//g_game->OnSize(g_windowInfo.width, g_windowInfo.height, wParam);
				break;
			}
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}