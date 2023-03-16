#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/Resource/LightBuffer.h"

namespace Graphics
{
	static void MakeLightMatrix(PerLight& perLight)
	{

		Math::Vector3 _right = Math::Vector3::Up.Cross(perLight._direction);
		Math::Vector3 _up = Math::Vector3::Up;

		_right.Normalize();

		_up = perLight._direction.Cross(_right);

		_up.Normalize();

		Math::Matrix _view = Math::Matrix::CreateLookAt(perLight._lightPosition, perLight._lightPosition + perLight._direction, _up);;
		Math::Matrix _proj;

		switch (perLight._type)
		{
			case static_cast<uint32>(LightType::Spot): // SpotLight
			{
				_proj = Math::Matrix::CreatePerspectiveFieldOfView(perLight._spotAngle, 1.0f, 0.1f, perLight._range);

				break;
			}
			case static_cast<uint32>(LightType::Directional):
			{
				// Cascade
				break;
			}
			case static_cast<uint32>(LightType::Point):
			{
				break;
			}
			default:
				assert(false);
				break;
		}
	
		perLight._shadowTransform = _view * _proj;
	}

	LightBuffer::LightBuffer(Graphics::RenderSystem* renderSystem)
		: ResourceBuffer(renderSystem, BufferType::Light)
	{

	}

	LightBuffer::~LightBuffer()
	{

	}

	void LightBuffer::GetSpotLightTransform(Math::Matrix& lightTM)
	{
		lightTM =
			Math::Matrix::CreateLookTo(
				m_PerLight._lightPosition,
				m_PerLight._direction,
				Math::Vector3::Up) *
			Math::Matrix::CreatePerspectiveFieldOfView(
				m_PerLight._spotAngle * Math::Deg2Rad
				, 1.0f, 0.1f, m_PerLight._range);
	}

	void LightBuffer::GetDirectionLightTransform(Math::Matrix& lightTM)
	{
		/*lightTM =
			Math::Matrix::CreateLookAt(
				m_PerLight._lightPosition,
				m_PerLight._lightPosition + m_PerLight._direction,
				Math::Vector3::Up) *
			Math::Matrix::CreateOrthographicOffCenter(
			);*/
	}

	void LightBuffer::GetPointLightTransform(Math::Matrix* lightTMs)
	{
		Math::Vector3 _up;

		Math::Matrix _views[6], _proj;
		// +X
		_up = { 0.0f, 1.0f, 0.0f };
		_views[0] = Math::Matrix::CreateLookTo(m_PerLight._lightPosition, Math::Vector3::Right, _up);

		// -X
		_up = { 0.0f, 1.0f, 0.0f };
		_views[1] = Math::Matrix::CreateLookTo(m_PerLight._lightPosition, Math::Vector3::Left, _up);

		// +Y
		_up = { 0.0f, 0.0f, -1.0f };
		_views[2] = Math::Matrix::CreateLookTo(m_PerLight._lightPosition, Math::Vector3::Up, _up);

		// -Y
		_up = { 0.0f, 0.0f, 1.0f };
		_views[3] = Math::Matrix::CreateLookTo(m_PerLight._lightPosition, Math::Vector3::Down, _up);

		// +Z
		_up = { 0.0f, 1.0f, 0.0f };
		_views[4] = Math::Matrix::CreateLookTo(m_PerLight._lightPosition, Math::Vector3::Forward, _up);

		// -Z
		_up = { 0.0f, 1.0f, 0.0f };
		_views[5] = Math::Matrix::CreateLookTo(m_PerLight._lightPosition, Math::Vector3::Backward, _up);

		_proj = Math::Matrix::CreatePerspectiveFieldOfView(90.f * Math::Deg2Rad, 1.0f, 0.1f, m_PerLight._range);

		lightTMs[0] = _views[0] * _proj;
		lightTMs[1] = _views[1] * _proj;
		lightTMs[2] = _views[2] * _proj;
		lightTMs[3] = _views[3] * _proj;
		lightTMs[4] = _views[4] * _proj;
		lightTMs[5] = _views[5] * _proj;
	}

	void LightBuffer::CreateRenderTexture()
	{
		// 라이트의 종류에 따른 랜더 타겟 생성 필요
		// 스팟 라이트는 하나만
		// 포인트 라이트는 랜더 타겟을 6개의 밉슬라이스로 생성 필요

		RenderTargetDesc _renderTargetDesc;

		//_renderTargetDesc._extend = (m_PerLight._type == static_cast<uint32>(LightType::Point)) ? { 512, 512 } : { 1024, 1024 };
		_renderTargetDesc._extend = { 512, 512 };

		AttachmentDesc _renderAttachDesc;

		_renderAttachDesc._renderTargetType = RenderTargetType::RenderTarget;

		_renderAttachDesc._mipLevel = 0;
		_renderAttachDesc._arrayLayer = m_PerLight._type == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_renderAttachDesc);

		AttachmentDesc _depthAttachDesc;

		_depthAttachDesc._renderTargetType = RenderTargetType::DepthStencil;

		_depthAttachDesc._mipLevel = 0;
		_depthAttachDesc._arrayLayer = m_PerLight._type == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_depthAttachDesc);
	}

	void LightBuffer::CreateRenderTarget()
	{
		// 라이트의 종류에 따른 랜더 타겟 생성 필요
		// 스팟 라이트는 하나만
		// 포인트 라이트는 랜더 타겟을 6개의 밉슬라이스로 생성 필요

		RenderTargetDesc _renderTargetDesc;

		//_renderTargetDesc._extend = (m_PerLight._type == static_cast<uint32>(LightType::Point)) ? { 512, 512 } : { 1024, 1024 };
		_renderTargetDesc._extend = { 512, 512 };

		AttachmentDesc _renderAttachDesc;

		_renderAttachDesc._renderTargetType = RenderTargetType::RenderTarget;

		_renderAttachDesc._mipLevel = 0;
		_renderAttachDesc._arrayLayer = m_PerLight._type == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_renderAttachDesc);

		AttachmentDesc _depthAttachDesc;

		_depthAttachDesc._renderTargetType = RenderTargetType::DepthStencil;

		_depthAttachDesc._mipLevel = 0;
		_depthAttachDesc._arrayLayer = m_PerLight._type == static_cast<uint32>(LightType::Point) ? 6 : 1;

		_renderTargetDesc._attachments.push_back(_depthAttachDesc);
	}
}