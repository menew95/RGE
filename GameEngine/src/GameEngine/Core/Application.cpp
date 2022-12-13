#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Application.h"

#include "GameEngine\Core\System\ComponentSystem.h"
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

		GameEngine::Core::Application& Application::Instance()
		{
			if (m_pApplication == nullptr)
			{
				m_pApplication = new Application();
				m_pApplication->Initallize();
			}

			return *m_pApplication;
		}

		void Application::Release()
		{
			if (m_pApplication != nullptr)
			{
				delete m_pApplication;
				m_pApplication = nullptr;
			}
		}

		void Application::Initallize()
		{
			m_pApplication->m_ComponentSystem = ComponentSystem::GetInstance();
			m_pApplication->m_SceneSystem = SceneSystem::GetInstance();
			m_pApplication->m_Resources = Resources::GetInstance();
		}

		bool Application::OnLoad(const tstring& dataPath)
		{
			bool _ret = false;

			return _ret;
		}

		bool Application::Update()
		{
			bool _ret = false;

			return _ret;
		}

		bool Application::Quit()
		{
			bool _ret = false;

			return _ret;
		}
	}
}