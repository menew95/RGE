#ifdef GAME_ENGINE_EXPORT
#ifndef GAME_ENGINE_API
#define GAME_ENGINE_API __declspec(dllexport)
#endif
#else
#ifndef GAME_ENGINE_API
#define GAME_ENGINE_API __declspec(dllimport)
#endif
#endif

namespace GameEngine
{
	namespace Core
	{
		class Application;
	}

	extern "C"
	{
		GAME_ENGINE_API Core::Application* CreateApplication();
		GAME_ENGINE_API void DeleteApplication(Core::Application*);

		GAME_ENGINE_API bool UpdateApplication();

		using ApplicationConstructor = Core::Application* (*)();
		using ApplicationDestructor = void (*)(Core::Application*);
	}
}