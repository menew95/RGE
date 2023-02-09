#include "Log/Log.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

bool Log::s_Initialized = false;
std::shared_ptr<spdlog::logger> Log::s_CoreLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::s_GraphicsLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::s_PhysicsLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::s_UILogger = nullptr;
std::shared_ptr<spdlog::logger> Log::s_FBXLoaderLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::s_EngineLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::s_EditorLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger = nullptr;

void Log::Initialize()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_CoreLogger = spdlog::stdout_color_mt("CORE");
	s_CoreLogger->set_level(spdlog::level::debug);

	s_GraphicsLogger = spdlog::stdout_color_mt("GRAPHICS");
	s_GraphicsLogger->set_level(spdlog::level::debug);

	s_PhysicsLogger = spdlog::stdout_color_mt("PHYSICS");
	s_PhysicsLogger->set_level(spdlog::level::debug);

	s_UILogger = spdlog::stdout_color_mt("UI");
	s_UILogger->set_level(spdlog::level::debug);

	s_FBXLoaderLogger = spdlog::stdout_color_mt("FBX_LOADER");
	s_FBXLoaderLogger->set_level(spdlog::level::debug);

	s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
	s_EngineLogger->set_level(spdlog::level::debug);

	s_EditorLogger = spdlog::stdout_color_mt("EDITOR");
	s_EditorLogger->set_level(spdlog::level::debug);

	s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
	s_ClientLogger->set_level(spdlog::level::debug);

	s_Initialized = true;
}
