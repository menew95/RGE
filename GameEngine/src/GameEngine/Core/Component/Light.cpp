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
		}

		void Light::Update()
		{
			auto _currPos = GetComponent<Transform>()->GetPosition();

			if (_currPos != m_PrevPosition && m_LightBuffer)
			{
				m_PrevPosition = _currPos;

				m_LightBuffer->SetLightPosition(m_PrevPosition);
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
			if (m_LightBuffer)
			{
				m_LightType = static_cast<LightType>(value);
				m_LightBuffer->SetLightType(value);
			}
		}

		void Light::SetLightColor(Math::Color& value)
		{
			if (m_LightBuffer)
			{
				m_LightColor = value;
				Math::Vector3 _color = { m_LightColor.x, m_LightColor.y, m_LightColor.z };
				m_LightBuffer->SetColor(_color);
			}
		}

		void Light::SetIntensity(float value)
		{
			if (m_LightBuffer)
			{
				m_Intensity = value;
				m_LightBuffer->SetIntensity(m_Intensity);
			}
		}

		void Light::SetRange(float value)
		{
			if (m_LightBuffer)
			{
				m_Range = value;
				m_LightBuffer->SetRange(m_Range);
			}
		}

		void Light::SetSpotAnlge(float value)
		{
			if (m_LightBuffer)
			{
				m_SpotAngle = value;
				m_LightBuffer->SetSpotAngle(m_SpotAngle);
			}
		}

		void Light::SetInnerSpotAngle(float value)
		{
			if (m_LightBuffer)
			{
				m_InnerSpotAngle = value;
				m_LightBuffer->SetFalloffAngle(value);
			}
		}

	}
}