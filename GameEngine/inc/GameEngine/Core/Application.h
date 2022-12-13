#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class ComponentSystem;

		class Application
		{
		public:
			Application();
			~Application();

			static Application& Instance();
			static void Release();

			static bool OnLoad(const tstring& dataPath);
			static bool Update();
			static bool Quit();

			void Initallize();
		protected:
			static Application* m_pApplication;

			class ComponentSystem* m_ComponentSystem;
			class SceneSystem* m_SceneSystem;
			class Resources* m_Resources;
		};
	}
}