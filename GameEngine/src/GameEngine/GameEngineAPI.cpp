#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\GameEngineAPI.h"
#include "GameEngine\Core\Application.h"

namespace GameEngine
{

	GAME_ENGINE_API Core::Application* CreateApplication()
	{
		auto _application = Core::Application::Instance();

		return _application;
	}

	GAME_ENGINE_API void DeleteApplication(Core::Application* application)
	{
		SAFE_DELETE(application);
	}

}