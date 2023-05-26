#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "Math/Math.h"

#define NUM_FRUSTUM_CORNERS 8
#define NUM_CASCADES 4

namespace Graphics
{
	struct Camera;
	struct CascadedLight;

	class GRAPHICSENGINE_DLL_DECLSPEC CameraBuffer : public ResourceBuffer
	{
	public:
		CameraBuffer(Graphics::RenderSystem* renderSystem);
		~CameraBuffer() override;

		void Update(Math::Vector3 pos, Math::Vector3 lookAt, Math::Vector3 right, Math::Vector3 up, float zNear, float zFar, float fov, float radio);

		void UpdateViwewMatrix();

		void UpdateProjMatrix();

		void UpdateCamera(Camera& camera);

		void UpdateCascadeShadow(Math::Vector3 directionalLightDir, CascadedLight& cascadedLight);

		inline void SetWorld(Math::Matrix world) { m_World = world; }

		inline Math::Matrix GetWorld() { return m_World; }
		inline Math::Matrix GetView() { return m_View; }
		inline Math::Matrix GetProj() { return m_Proj; }

		void GetCascadedInfo();
	public:
		Math::Vector3 m_CameraPosition = { 0, 0, 0 };
		Math::Vector3 m_Right = { 1, 0, 0 };
		Math::Vector3 m_Up = { 0, 1, 0 };
		Math::Vector3 m_LookAt = { 0, 0, 1 };

		float m_Near = 0.1f;
		float m_Far = 1000.0f;

		float m_FieldOfView = 90; // Degree
		float m_AspectRadio = 1.0f;

		float m_NearWindowHeight = 1.0f;
		float m_FarWindowHeight = 1.0f;

		Math::Matrix m_World;
		Math::Matrix m_View;
		Math::Matrix m_Proj;

		float m_cascadeEnd[NUM_CASCADES + 1];
		struct ShadowOrthoProjInfo
		{
			float _right;
			float _left;
			float _bottum;
			float _top;
			float _far;
			float _near;
		};

		ShadowOrthoProjInfo m_ShadowOrthoProjInfo[NUM_CASCADES];
	};
}