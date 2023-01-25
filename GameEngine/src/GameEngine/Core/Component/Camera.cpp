#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/Component/Camera.h"

#include "GameEngine/Core/Component/Transform.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GraphicsEngine/Resource/CameraBuffer.h"


std::weak_ptr<GameEngine::Core::Camera> GameEngine::Core::Camera::m_MainCamera;

namespace GameEngine
{
	namespace Core
	{

		Camera::Camera(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName /*= TEXT("Camera")*/)
			: Component(gameObject, componentName)
		{

		}

		Camera::~Camera()
		{
			if (m_MainCamera.expired())
			{
				m_MainCamera.reset();
			}
		}

		void Camera::Awake()
		{
			if (m_MainCamera.lock() == nullptr)
			{
				m_MainCamera = shared_from_this();
			}

			m_CameraBuffer = GraphicsSystem::GetInstance()->CreateCameraBuffer();
		}

		void Camera::Update()
		{
			auto _transform = GetTransform();
			
			Math::Vector3 _pos = _transform->GetPosition();
			Math::Vector3 _forward = _transform->GetForward();
			Math::Vector3 _right = _transform->GetRight();
			Math::Vector3 _up = _transform->GetUp();

			m_CameraBuffer->Update(_pos, _forward, _right, _up, m_Near, m_Far, m_FieldOfView, m_AspectRadio);
		}
	}
}