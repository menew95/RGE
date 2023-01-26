#pragma once

#include "GameEngine/Core/Component/Component.h"

namespace Graphics
{
	class LightBuffer;
}

namespace GameEngine
{
	namespace Core
	{
		enum class LightType
		{
			Spot = 0,
			Directional = 1,
			Point = 2,
		};

		class GAME_ENGINE_API Light : public Component
		{
		public:
			Light(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName = TEXT("Light"));
			~Light() override;

			void Awake() override;
			void Update() override;
			void OnEnable() override;
			void OnDisable() override;

			void SetLightType(uint32 value);
			void SetLightColor(Math::Color& value);
			void SetIntensity(float value);
			void SetRange(float value);
			void SetSpotAnlge(float value);
			void SetInnerSpotAngle(float value);

			inline uint32 GetLightType() { return static_cast<uint32>(m_LightType); }
			inline Math::Color& GetLightColor() { return m_LightColor; }
			inline float GetIntensity() { return m_Intensity; }
			inline float GetRange() { return m_Range; }
			inline float GetSpotAnlge() { return m_SpotAngle; }
			inline float GetInnerSpotAngle() { return m_InnerSpotAngle; }

		private:
			Graphics::LightBuffer* m_LightBuffer = nullptr;

			Math::Vector3 m_PrevPosition		= Math::Vector3::Zero;

			LightType m_LightType				= LightType::Directional;
			Math::Color m_LightColor			= Math::Color::White;
			float m_Intensity					= 0.5f;
			float m_Range						= 10.0f;
			float m_SpotAngle					= 30.0f;
			float m_InnerSpotAngle				= 21.80208f;	// _fallOffAngle
		};
	}
}