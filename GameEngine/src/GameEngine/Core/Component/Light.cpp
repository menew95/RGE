#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/Component/Light.h"
#include "GameEngine/Core/Component/Transform.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GraphicsEngine/Resource/LightBuffer.h"

namespace GameEngine
{
	namespace Core
	{

		Light::Light(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName /*= TEXT("Light")*/)
			: Component(gameObject, componentName)
		{

		}

		Light::~Light()
		{
			GraphicsSystem::GetInstance()->DeleteLightBuffer(m_LightBuffer);

			m_LightBuffer = nullptr;
		}


		void Light::Awake()
		{
			m_LightBuffer = GraphicsSystem::GetInstance()->CreateLightBuffer();

			assert(m_LightBuffer != nullptr);

			m_LightBuffer->SetDirection(GetComponent<Transform>()->GetForward());

			m_LightBuffer->SetLightType(static_cast<uint32>(m_LightType));

			Math::Vector3 _color = { m_LightColor.x, m_LightColor.y, m_LightColor.z };

			m_LightBuffer->SetColor(_color);
			m_LightBuffer->SetIntensity(m_Intensity);
			m_LightBuffer->SetRange(m_Range);
			m_LightBuffer->SetSpotAngle(m_SpotAngle);
			m_LightBuffer->SetFalloffAngle(m_InnerSpotAngle);

			switch (m_LightType)
			{
				case GameEngine::Core::LightType::Spot:
					m_LightBuffer->SetRange(m_Range);
					m_LightBuffer->SetFalloffAngle(m_InnerSpotAngle);

					break;
				case GameEngine::Core::LightType::Directional:

					break;
				case GameEngine::Core::LightType::Point:

					break;
				default:
					break;
			}
		}

		void Light::Update()
		{
			auto _currPos = GetComponent<Transform>()->GetPosition();

			if (_currPos != m_PrevPosition && m_LightBuffer)
			{
				m_PrevPosition = _currPos;

				m_LightBuffer->SetLightPosition(m_PrevPosition);

				m_LightBuffer->SetDirection(GetComponent<Transform>()->GetForward());
			}
		}

		void Light::OnEnable()
		{
			if (m_LightBuffer)
			{
				m_LightBuffer->SetEnable(true);
			}
		}

		void Light::OnDisable()
		{
			if (m_LightBuffer)
			{
				m_LightBuffer->SetEnable(false);
			}
		}

		void Light::SetLightType(uint32 value)
		{
			
			m_LightType = static_cast<LightType>(value);
			if (m_LightBuffer)
			{
				m_LightBuffer->SetLightType(value);
			}
		}

		void Light::SetLightColor(Math::Color value)
		{
			m_LightColor = value;

			if (m_LightBuffer)
			{
				Math::Vector3 _color = { m_LightColor.x, m_LightColor.y, m_LightColor.z };
				m_LightBuffer->SetColor(_color);
			}
		}

		void Light::SetIntensity(float value)
		{
			m_Intensity = value;

			if (m_LightBuffer)
			{
				m_LightBuffer->SetIntensity(m_Intensity);
			}
		}

		void Light::SetRange(float value)
		{
			m_Range = value;

			if (m_LightBuffer)
			{
				m_LightBuffer->SetRange(m_Range);
			}
		}

		void Light::SetSpotAnlge(float value)
		{
			m_SpotAngle = value;

			if (m_LightBuffer)
			{
				m_LightBuffer->SetSpotAngle(m_SpotAngle);
			}
		}

		void Light::SetInnerSpotAngle(float value)
		{
			m_InnerSpotAngle = value;

			if (m_LightBuffer)
			{
				m_LightBuffer->SetFalloffAngle(value);
			}
		}

	}
}