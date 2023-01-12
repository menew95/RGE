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
			m_pApplication->m_Window = Window::GetInstance();
			m_pApplication->m_Window->Initialize(showCmd, hInstance, windowClassName, windowName, width, height);

			m_pApplication->m_ComponentSystem = ComponentSystem::GetInstance();
			m_pApplication->m_SceneSystem = SceneSystem::GetInstance();
			m_pApplication->m_Resources = Resources::GetInstance();
			m_pApplication->m_GameTime = Time::GetInstance();
			m_pApplication->m_Input = Input::GetInstance();

			
			m_Input->Initialize(m_pApplication->m_Window->GetWindowInfo()._hWnd);
			m_GameTime->Initialize();

			m_GraphicsSystem = GraphicsSystem::GetInstance();
			m_GraphicsSystem->Initialize();

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