#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"
#include "GraphicsEngine/Resource/ConstantBuffer.h"

namespace Graphics
{

	CameraBuffer::CameraBuffer(Graphics::RenderSystem* renderSystem)
		: ResourceBuffer(renderSystem, BufferType::Camera)
	{

	}

	CameraBuffer::~CameraBuffer()
	{

	}

	void CameraBuffer::Update(Math::Vector3 pos, Math::Vector3 lookAt, Math::Vector3 right, Math::Vector3 up, float zNear, float zFar, float fov, float radio)
	{
		m_CameraPosition = pos;
		m_Right = right;
		m_Up = up;
		m_LookAt = lookAt;

		m_Near = zNear;
		m_Far = zFar;
		m_FieldOfView = fov;
		m_AspectRadio = radio;
	}

	void CameraBuffer::UpdateViwewMatrix()
	{
		m_View = Math::Matrix::CreateLookAt(m_CameraPosition, m_CameraPosition + m_LookAt, m_Up);
	}

	void CameraBuffer::UpdateProjMatrix()
	{
		m_NearWindowHeight = 2.0f * m_Near * tanf(0.5f * m_FieldOfView);
		m_FarWindowHeight = 2.0f * m_Far * tanf(0.5f * m_FieldOfView);

		m_Proj = Math::Matrix::CreatePerspectiveFieldOfView(m_FieldOfView, m_AspectRadio, m_Near, m_Far);
	}

	void CameraBuffer::UpdateCamera(Camera& camera)
	{
		UpdateViwewMatrix();
		UpdateProjMatrix();

		camera._camWorld = m_CameraPosition;
		camera._view = m_View;
		camera._proj = m_Proj;
		camera._projInv = m_Proj.Invert().Transpose();

		camera._viewToTexSpace =
		{
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
		};
	}

}