#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class ComponentSystem;

		class GAME_ENGINE_API Application
		{
		public:
			Application();
			~Application();

			static Application* Instance();
			static void Release();

			bool OnLoad(const tstring& dataPath);
			bool Update();
			bool Quit();

			bool Initallize(bool showCmd,
				HINSTANCE hInstance,
				const tstring& windowClassName,
				const tstring& windowName,
				UINT width,
				UINT height);
		protected:
			static Application* m_pApplication;

			class Window* m_Window;
			class ComponentSystem* m_ComponentSystem;
			class SceneSystem* m_SceneSystem;
			class Resources* m_Resources;

			class Time* m_GameTime;
			class Input* m_Input;
		};
	}
}