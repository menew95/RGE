#pragma once

#include "GameEngine/Core/Component/Component.h"

namespace Graphics
{
	class CameraBuffer;
}

namespace GameEngine
{
	namespace Core
	{
		class GAME_ENGINE_API Camera : public Component, public std::enable_shared_from_this<Camera>
		{
		public:
			Camera(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName = TEXT("Camera"));
			virtual ~Camera();


			void Awake() override;
			//void Start() override;
			void Update() override;
			//void LateUpdate() override;
			//void FixedUpdate() override;
			//void OnEnable() override;
			//void OnDisable() override;

			static std::shared_ptr<Camera> GetMainCamera() { return m_MainCamera.lock(); }

		private:
			float m_Near = 0.1f;
			float m_Far = 1000.0f;
			float m_FieldOfView = 90;
			float m_AspectRadio = 1.0f;

			static std::weak_ptr<Camera> m_MainCamera;

			Graphics::CameraBuffer* m_CameraBuffer = nullptr;

			RTTR_ENABLE(Component)

			RTTR_REGISTRATION_FRIEND
		};
	}
}