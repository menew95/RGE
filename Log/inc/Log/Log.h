#pragma once

#include <memory>
#include <spdlog/logger.h>

#if defined(_MSC_VER) && defined(LOG_EXPORT)
#ifndef LOG_DLL_DECLSPEC
#define LOG_DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef LOG_DLL_DECLSPEC
#define LOG_DLL_DECLSPEC dll_import
#endif
#endif

namespace spdlog
{
	class logger;
}

class LOG_DLL_DECLSPEC Log
{
public:
	static void Initialize();

	static bool IsInitialized() { return s_Initialized; }

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	// log level은 중요도 오름차순으로 {trace, debug, info, warn, err, critical} 입니다.
	// (오른쪽에 있는 것일수록 치명적으로 중요한 로그라는 것을 나타냅니다.)

	/* Core */

	template <typename ... Args>
	inline static void Core_Trace(std::string _format, Args ... _args) { s_CoreLogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void Core_Debug(std::string _format, Args ... _args) { s_CoreLogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void Core_Info(std::string _format, Args ... _args) { s_CoreLogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void Core_Warn(std::string _format, Args ... _args) { s_CoreLogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void Core_Error(std::string _format, Args ... _args) { s_CoreLogger->error(_format, _args ...); }

	/* Graphics */

	template <typename ... Args>
	inline static void Graphics_Trace(std::string _format, Args ... _args) { s_GraphicsLogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void Graphics_Debug(std::string _format, Args ... _args) { s_GraphicsLogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void Graphics_Info(std::string _format, Args ... _args) { s_GraphicsLogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void Graphics_Warn(std::string _format, Args ... _args) { s_GraphicsLogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void Graphics_Error(std::string _format, Args ... _args) { s_GraphicsLogger->error(_format, _args ...); }

	/* Physics */

	template <typename ... Args>
	inline static void Physics_Trace(std::string _format, Args ... _args) { s_PhysicsLogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void Physics_Debug(std::string _format, Args ... _args) { s_PhysicsLogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void Physics_Info(std::string _format, Args ... _args) { s_PhysicsLogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void Physics_Warn(std::string _format, Args ... _args) { s_PhysicsLogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void Physics_Error(std::string _format, Args ... _args) { s_PhysicsLogger->error(_format, _args ...); }

	/* UI */

	template <typename ... Args>
	inline static void UI_Trace(std::string _format, Args ... _args) { s_UILogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void UI_Debug(std::string _format, Args ... _args) { s_UILogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void UI_Info(std::string _format, Args ... _args) { s_UILogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void UI_Warn(std::string _format, Args ... _args) { s_UILogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void UI_Error(std::string _format, Args ... _args) { s_UILogger->error(_format, _args ...); }

	/* FBX Loader */

	template <typename ... Args>
	inline static void FBXLoader_Trace(std::string _format, Args ... _args) { s_FBXLoaderLogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void FBXLoader_Debug(std::string _format, Args ... _args) { s_FBXLoaderLogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void FBXLoader_Info(std::string _format, Args ... _args) { s_FBXLoaderLogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void FBXLoader_Warn(std::string _format, Args ... _args) { s_FBXLoaderLogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void FBXLoader_Error(std::string _format, Args ... _args) { s_FBXLoaderLogger->error(_format, _args ...); }

	/* Engine */

	template <typename ... Args>
	inline static void Engine_Trace(std::string _format, Args ... _args) { s_EngineLogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void Engine_Debug(std::string _format, Args ... _args) { s_EngineLogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void Engine_Info(std::string _format, Args ... _args) { s_EngineLogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void Engine_Warn(std::string _format, Args ... _args) { s_EngineLogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void Engine_Error(std::string _format, Args ... _args) { s_EngineLogger->error(_format, _args ...); }

	/* Editor */

	template <typename ... Args>
	inline static void Editor_Trace(std::string _format, Args ... _args) { s_EditorLogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void Editor_Debug(std::string _format, Args ... _args) { s_EditorLogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void Editor_Info(std::string _format, Args ... _args) { s_EditorLogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void Editor_Warn(std::string _format, Args ... _args) { s_EditorLogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void Editor_Error(std::string _format, Args ... _args) { s_EditorLogger->error(_format, _args ...); }

	/* Client */

	template <typename ... Args>
	inline static void Client_Trace(std::string _format, Args ... _args) { s_ClientLogger->trace(_format, _args ...); }

	template <typename ... Args>
	inline static void Client_Debug(std::string _format, Args ... _args) { s_ClientLogger->debug(_format, _args ...); }

	template <typename ... Args>
	inline static void Client_Info(std::string _format, Args ... _args) { s_ClientLogger->info(_format, _args ...); }

	template <typename ... Args>
	inline static void Client_Warn(std::string _format, Args ... _args) { s_ClientLogger->warn(_format, _args ...); }

	template <typename ... Args>
	inline static void Client_Error(std::string _format, Args ... _args) { s_ClientLogger->error(_format, _args ...); }

private:
	static bool s_Initialized;
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_GraphicsLogger;
	static std::shared_ptr<spdlog::logger> s_PhysicsLogger;
	static std::shared_ptr<spdlog::logger> s_UILogger;
	static std::shared_ptr<spdlog::logger> s_FBXLoaderLogger;
	static std::shared_ptr<spdlog::logger> s_EngineLogger;
	static std::shared_ptr<spdlog::logger> s_EditorLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};