#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "Math/Math.h"

namespace Graphics
{
	struct Camera;

	class GRAPHICSENGINE_DLL_DECLSPEC CameraBuffer : public ResourceBuffer
	{
	public:
		CameraBuffer(Graphics::RenderSystem* renderSystem);
		~CameraBuffer() override;

		void Update(Math::Vector3 pos, Math::Vector3 lookAt, Math::Vector3 right, Math::Vector3 up, float zNear, float zFar, float fov, float radio);

		void UpdateViwewMatrix();

		void UpdateProjMatrix();

		void UpdateCamera(Camera& camera);
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

		Math::Matrix m_View;
		Math::Matrix m_Proj;
	};
}