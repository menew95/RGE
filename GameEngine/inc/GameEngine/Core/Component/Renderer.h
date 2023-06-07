#pragma once
#include "GameEngine\Core\Component\Component.h"

#include "Math\Math.h"

#include "GraphicsEngine/RenderObject.h"

namespace GameEngine
{
	namespace Core
	{
		class GAME_ENGINE_API Renderer
			: public Component
		{
		public:
			Renderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("Renderer"));
			virtual ~Renderer() override;

			virtual void Link() {}
			virtual void Unlink() {}

			virtual void OnEnable();
			virtual void OnDisalbe();

			void SetShadowCasting(bool value);
			inline bool GetSetShadowCasting() { return m_bIsShadowCasting; }

		protected:
			Graphics::RenderObject* m_RenderObject;

			struct PerObjectData
			{
				Math::Matrix _world;
				Math::Matrix _worldInvTranspose;
			};

			PerObjectData _perObject;

			bool m_bIsShadowCasting = true;

			RTTR_ENABLE(Component)

			RTTR_REGISTRATION_FRIEND
		};
	}
}

