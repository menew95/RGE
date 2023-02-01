#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Window.h"
#include "GameEngine\SystemTimer.h"
#include "GameEngine\Time.h"
#include "GameEngine\Input.h"

#include "GameEngine\Core\Application.h"
#include "GameEngine\Core\System\ComponentSystem.h"
#include "GameEngine\Core\System\GraphicsSystem.h"
#include "GameEngine\Core\System\SceneSystem.h"
#include "GameEngine\Core\System\Resources.h"

#include "Log/Log.h"

namespace GameEngine
{
	namespace Core
	{
		Application* Application::m_pApplication = nullptr;

		Application::Application()
		{

		}

		Application::~Application()
		{

		}

		GameEngine::Core::Application* Application::Instance()
		{
			if (m_pApplication == nullptr)
			{
				m_pApplication = new Application();
			}

			return m_pApplication;
		}

		void Application::Release()
		{
			if (m_pApplication != nullptr)
			{
				delete m_pApplication;
				m_pApplication = nullptr;
			}
		}

		bool Application::Initallize(bool showCmd,
			HINSTANCE hInstance,
			const tstring& windowClassName,
			const tstring& windowName,
			UINT width,
			UINT height)
		{
			if (Log::IsInitialized() == false)
			{
				Log::Initialize();
			}

			m_pApplication->m_Window = Window::GetInstance();
			m_pApplication->m_Window->Initialize(showCmd, hInstance, windowClassName, windowName, width, height);
			Log::Client_Info("Window create succeed");

			m_GraphicsSystem = GraphicsSystem::GetInstance();
			m_GraphicsSystem->Initialize();
			Log::Client_Info("Graphics System init succeed");

			m_pApplication->m_ComponentSystem = ComponentSystem::GetInstance();

			m_pApplication->m_SceneSystem = SceneSystem::GetInstance();

			m_pApplication->m_Resources = Resources::GetInstance();
			m_pApplication->m_Resources->Init();
			Log::Client_Info("Resource System init succeed");

			m_pApplication->m_GameTime = Time::GetInstance();
			m_GameTime->Initialize();
			Log::Client_Info("Time init succeed");

			m_pApplication->m_Input = new Input();

			Input::Instance->Initialize(m_pApplication->m_Window->GetWindowInfo()._hWnd);
			Log::Client_Info("Input init succeed");

			m_pApplication->m_SceneSystem->Initialize();
			m_pApplication->m_SceneSystem->LoadScene(0);
			Log::Client_Info("Scene load succeed");

			return true;
		}

		bool Application::OnLoad(const tstring& dataPath)
		{
			bool _ret = false;


			return _ret;
		}

		bool Application::Update()
		{
			bool _ret = false;

			SystemTimer _sysTimer;

			_sysTimer.Lap();
			{
				m_GameTime->PreTick();
				m_Input->PreTick();

				m_ComponentSystem->UpdateComponent();

				m_ComponentSystem->RenderComponent();
			}

			_sysTimer.Lap();
			{
				m_GraphicsSystem->Render();
			}


			return _ret;
		}

		bool Application::Quit()
		{
			bool _ret = false;

			m_ComponentSystem->ClearComponentsList();

			return _ret;
		}
	}
}