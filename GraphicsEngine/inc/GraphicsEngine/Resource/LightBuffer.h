#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "GraphicsEngine/Resource/ConstantBuffer.h"

#include "Math/Math.h"


namespace Graphics
{
	struct Camera;
	class RenderTarget;

	enum class LightType
	{
		Spot		= 0,
		Directional	= 1,
		Point		= 2,
	};

	class GRAPHICSENGINE_DLL_DECLSPEC LightBuffer : public ResourceBuffer
	{
	public:
		LightBuffer(Graphics::RenderSystem* renderSystem);
		~LightBuffer() override;

		inline bool GetEnable() { return m_Enable; }
		inline void SetEnable(bool value) { m_Enable = value; }

		inline PerLight& GetPerLight() { return m_PerLight; }

		inline uint32 GetLightType() { return m_PerLight._type; }
		inline void SetLightType(uint32 value) { m_PerLight._type = value; }

		inline float GetRange() { return m_PerLight._range; }
		inline void SetRange(float value) { m_PerLight._range = value; }

		inline float GetFallOff() { return m_PerLight._fallOff; }
		inline void SetFallOff(float value) { m_PerLight._fallOff = value; }

		inline Math::Vector3& GetLightPosition() { return m_PerLight._lightPosition; }
		inline void SetLightPosition(Math::Vector3& value) { m_PerLight._lightPosition = value; }

		inline float GetSpotAngle() { return m_PerLight._spotAngle; }
		inline void SetSpotAngle(float value) { m_PerLight._spotAngle = value; }

		inline Math::Vector3& GetDirection() { return m_PerLight._direction; }
		inline void SetDirection(Math::Vector3& value) { m_PerLight._direction = value; }

		inline float GetFalloffAngle() { return m_PerLight._fallOffAngle; }
		inline void SetFalloffAngle(float value) { m_PerLight._fallOffAngle = value; }

		inline Math::Vector3& GetColor() { return m_PerLight._color; }
		inline void SetColor(Math::Vector3& value) { m_PerLight._color = value; }

		inline float GetIntensity() { return m_PerLight._intensity; }
		inline void SetIntensity(float value) { m_PerLight._intensity = value; }

		inline RenderTarget* GetRenderTarget() { return m_RenderTarget; }

		void GetSpotLightTransform(Math::Matrix& lightTM);
		void GetDirectionLightTransform(Math::Matrix& lightTM);
		void GetPointLightTransform(Math::Matrix* lightTMs);

	private:
		void CreateRenderTexture();

		void CreateRenderTarget();

		bool m_Enable = false;

		PerLight m_PerLight;

		RenderTarget* m_RenderTarget = nullptr;
		Texture* m_ShadowMap = nullptr;
	};
}
