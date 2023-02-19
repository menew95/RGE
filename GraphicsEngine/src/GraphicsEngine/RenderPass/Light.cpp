#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderPass/Light.h"
#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/Resource/LightBuffer.h"

namespace Graphics
{
	
	Light::Light(RenderSystem* renderSystem, ResourceManager* resourceManager)
		: m_RenderSystem(renderSystem)
		, m_ResourceManager(resourceManager)
	{

	}

	Light::~Light()
	{

	}

	std::shared_ptr<Graphics::LightBuffer> Light::AddLight()
	{
		std::shared_ptr<LightBuffer> _newLight = std::make_shared<LightBuffer>(m_RenderSystem);

		m_LightBuffers.push_back(_newLight);

		return _newLight;
	}

	void Light::GetLightingData(struct Lighting& perLightFrame)
	{
		perLightFrame._dirLightCount = 0;
		perLightFrame._pointLightCount = 0;
		perLightFrame._spotLightCount = 0;
		for (uint32 i = 0; i < m_LightBuffers.size(); i++)
		{
			if (m_LightBuffers[i]->GetEnable())
			{
				const auto& _perLight = m_LightBuffers[i]->GetPerLight();

				// 0 : spot 1 : dir 2 : point
				switch (_perLight._type)
				{
					case 0:
					{
						SpotLight _spotLight;

						_spotLight._range = _perLight._range;
						_spotLight._fallOff = _perLight._fallOff;

						_spotLight._position = _perLight._lightPosition;
						_spotLight._spotAngle = _perLight._spotAngle;

						_spotLight._direction = _perLight._direction;
						_spotLight._fallOffAngle = _perLight._fallOffAngle;

						_spotLight._color = _perLight._color;
						_spotLight._power = _perLight._intensity;

						m_LightBuffers[i]->GetSpotLightTransform(_spotLight._lightTransform);
						
						perLightFrame._spotLight[perLightFrame._spotLightCount] = _spotLight;

						perLightFrame._spotLightCount++;

						break;
					}
					case 1:
					{
						DirectionLight _dirLight;

						_dirLight._direction = _perLight._direction;
						_dirLight._color = _perLight._color;
						_dirLight._power = _perLight._intensity;

						m_LightBuffers[i]->GetDirectionLightTransform(_dirLight._lightTransform);

						perLightFrame._directionLight[perLightFrame._dirLightCount] = _dirLight;

						perLightFrame._dirLightCount++;

						break;
					}
					case 2:
					{
						PointLight _pointLight;

						_pointLight._position = _perLight._lightPosition;

						_pointLight._range = _perLight._range;

						_pointLight._color = _perLight._color;

						_pointLight._fallOff = _perLight._fallOff;

						_pointLight._power = _perLight._intensity;

						m_LightBuffers[i]->GetPointLightTransform(_pointLight._lightTransform);
						
						perLightFrame._pointLight[perLightFrame._pointLightCount] = _pointLight;

						perLightFrame._pointLightCount++;

						break;
					}
					default:
						break;
				}
			}
		}
	}

	void Light::UpdateLightTexture()
	{
		ImageDesc _imgDesc;
		TextureRegion _region;
		m_RenderSystem->WriteTexture(*m_LightTexture, _region, _imgDesc);
	}

	void Light::Init()
	{
		m_LightTexture = m_ResourceManager->GetTexture(TEXT("LightTexture"));
	}
}