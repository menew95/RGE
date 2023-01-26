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

	void LightBuffer::UpdateShadowTransform()
	{

	}

	void LightBuffer::CreateRenderTarget()
	{
		// ����Ʈ�� ������ ���� ���� Ÿ�� ���� �ʿ�
		// ���� ����Ʈ�� �ϳ���
		// ����Ʈ ����Ʈ�� ���� Ÿ���� 6���� �ӽ����̽��� ���� �ʿ�

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