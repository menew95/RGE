#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/CameraBuffer.h"
#include "GraphicsEngine/Resource/ConstantBuffer.h"

#include <limits>

namespace Graphics
{

	CameraBuffer::CameraBuffer(Graphics::RenderSystem* renderSystem, uuid uuid)
		: ResourceBuffer(renderSystem, BufferType::Camera, uuid)
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
		m_FieldOfView = fov * Math::Deg2Rad;
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
		camera._projInv = m_Proj.Invert();

		camera._viewToTexSpace =
		{
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
		};

		camera._viewToTextureSpaceMatrix = m_Proj * camera._viewToTexSpace;

		Math::Matrix _camWorldMatrix = Math::Matrix::CreateTranslation(m_CameraPosition);

		camera._worldViewProj = _camWorldMatrix * m_View * m_Proj;

		camera._near = m_Near;
		camera._far = m_Far;
	}

	void CameraBuffer::UpdateCascadeShadow(Math::Vector3 directionalLightDir, CascadedLight& cascadedLight)
	{
		m_cascadeEnd[0] = m_Near;
		m_cascadeEnd[1] = 10.0f,
		m_cascadeEnd[2] = 20.0f,
		m_cascadeEnd[3] = 40.f;
		//Todo 너무 크다
		m_cascadeEnd[4] = 100.f;
		
		cascadedLight._cascadeEndClipSpace[0].x = m_cascadeEnd[1];
		cascadedLight._cascadeEndClipSpace[1].x = m_cascadeEnd[2];
		cascadedLight._cascadeEndClipSpace[2].x = m_cascadeEnd[3];
		cascadedLight._cascadeEndClipSpace[3].x = m_cascadeEnd[4];

		// Get the inverse of the view transform
		Math::Matrix Cam = m_View;
		Math::Matrix CamInv = m_View.Invert();

		// Get the light space transform
		Math::Matrix LightM = Math::Matrix::CreateLookAt(Math::Vector3(0.0f, 0.0f, 0.0f), directionalLightDir, Math::Vector3(0.0f, 1.0f, 0.0f));

		float tanHalfHFOV = std::tanf((m_FieldOfView / 2.0f));
		float tanHalfVFOV = std::tanf(((m_FieldOfView * m_AspectRadio) / 2.0f));

		for (uint i = 0; i < NUM_CASCADES; i++) {
			float xn = m_cascadeEnd[i] * tanHalfHFOV;
			float xf = m_cascadeEnd[i + 1] * tanHalfHFOV;
			float yn = m_cascadeEnd[i] * tanHalfVFOV;
			float yf = m_cascadeEnd[i + 1] * tanHalfVFOV;

			Math::Vector4 frustumCorners[NUM_FRUSTUM_CORNERS] = {
				// near face
				Math::Vector4(xn, yn, m_cascadeEnd[i], 1.0),
				Math::Vector4(-xn, yn, m_cascadeEnd[i], 1.0),
				Math::Vector4(xn, -yn, m_cascadeEnd[i], 1.0),
				Math::Vector4(-xn, -yn, m_cascadeEnd[i], 1.0),

				// far face
				Math::Vector4(xf, yf, m_cascadeEnd[i + 1], 1.0),
				Math::Vector4(-xf, yf, m_cascadeEnd[i + 1], 1.0),
				Math::Vector4(xf, -yf, m_cascadeEnd[i + 1], 1.0),
				Math::Vector4(-xf, -yf, m_cascadeEnd[i + 1], 1.0)
			};
			Math::Vector4 frustumCornersL[NUM_FRUSTUM_CORNERS];

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::min();
			float minZ = std::numeric_limits<float>::max();
			float maxZ = std::numeric_limits<float>::min();

			for (uint j = 0; j < NUM_FRUSTUM_CORNERS; j++) {

				// Transform the frustum coordinate from view to world space
				Math::Vector4 vW = Math::Vector4::Transform(frustumCorners[j], CamInv);

				// Transform the frustum coordinate from world to light space
				frustumCornersL[j] = Math::Vector4::Transform(vW, LightM);

				minX = std::min(minX, frustumCornersL[j].x);
				maxX = std::max(maxX, frustumCornersL[j].x);
				minY = std::min(minY, frustumCornersL[j].y);
				maxY = std::max(maxY, frustumCornersL[j].y);
				minZ = std::min(minZ, frustumCornersL[j].z);
				maxZ = std::max(maxZ, frustumCornersL[j].z);
			}

			m_ShadowOrthoProjInfo[i]._right = maxX;
			m_ShadowOrthoProjInfo[i]._left = minX;
			m_ShadowOrthoProjInfo[i]._bottum = minY;
			m_ShadowOrthoProjInfo[i]._top = maxY;
			m_ShadowOrthoProjInfo[i]._far = maxZ;
			m_ShadowOrthoProjInfo[i]._near = minZ;

			cascadedLight._lightTransform[i] = LightM * Math::Matrix::CreateOrthographicOffCenter(
				m_ShadowOrthoProjInfo[i]._left,
				m_ShadowOrthoProjInfo[i]._right,
				m_ShadowOrthoProjInfo[i]._bottum,
				m_ShadowOrthoProjInfo[i]._top,
				m_ShadowOrthoProjInfo[i]._near,
				m_ShadowOrthoProjInfo[i]._far
			);
		}

	}

}