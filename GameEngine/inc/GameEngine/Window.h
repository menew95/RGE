#pragma once

#include "Common.h"

namespace GameEngine
{
	struct WindowInfo
	{
		HINSTANCE _hInstance = 0;
		HWND _hWnd = 0;
		uint32 _width = 1;
		uint32 _height = 1;
	};

	struct WindowDesc
	{
		bool _showCmd;
		HINSTANCE _hInstance;
		const tstring& _windowClassName;
		const tstring& _windowName;
		UINT _width;
		UINT _height;
	};

	class GAME_ENGINE_API Window
	{
		DECLARE_SINGLETON_CLASS(Window)
	public:

		HRESULT Initialize(bool showCmd,
			HINSTANCE hInstance,
			const tstring& windowClassName,
			const tstring& windowName,
			UINT width,
			UINT height);

		WindowInfo& GetWindowInfo() { return m_WindowInfo; }

	private:
		ATOM RegisterWindow(HINSTANCE hInstance);

		HRESULT InitializeWindow(bool showCmd);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		std::wstring m_WindowClassNameW;
		tstring m_WindowClassName;
		tstring m_WindowName;

		WindowInfo m_WindowInfo;

	};
}