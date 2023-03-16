#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderPass/Light.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"
#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/Resource/LightBuffer.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"

#include "GraphicsModule/Core/RenderTarget.h"

namespace Graphics
{
	
	Light::Light(RenderSystem* renderSystem, CommandBuffer* command, ResourceManager* resourceManager)
		: m_RenderSystem(renderSystem)
		, m_CommandBuffer(command)
		, m_ResourceManager(resourceManager)
	{
		Init();
	}

	Light::~Light()
	{

	}

	LightBuffer* Light::AddLight()
	{
		std::shared_ptr<LightBuffer> _newLight = std::make_shared<LightBuffer>(m_RenderSystem);

		m_LightBuffers.push_back(_newLight);

		return _newLight.get();
	}

	Lighting* Light::GetLightingData()
	{
		m_Lighting._dirLightCount = 0;
		m_Lighting._pointLightCount = 0;
		m_Lighting._spotLightCount = 0;

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
						_spotLight._spotAngle = _perLight._spotAngle * Math::Deg2Rad;

						_spotLight._direction = _perLight._direction;
						_spotLight._fallOffAngle = _perLight._fallOffAngle * Math::Deg2Rad;

						_spotLight._color = _perLight._color;
						_spotLight._power = _perLight._intensity;

						m_LightBuffers[i]->GetSpotLightTransform(_spotLight._lightTransform);
						
						m_Lighting._spotLight[m_Lighting._spotLightCount] = _spotLight;

						m_Lighting._spotLightCount++;

						break;
					}
					case 1:
					{
						DirectionLight _dirLight;

						_dirLight._direction = _perLight._direction;
						_dirLight._color = _perLight._color;
						_dirLight._power = _perLight._intensity;

						m_LightBuffers[i]->GetDirectionLightTransform(_dirLight._lightTransform);

						m_Lighting._directionLight[m_Lighting._dirLightCount] = _dirLight;

						m_Lighting._dirLightCount++;

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
						
						m_Lighting._pointLight[m_Lighting._pointLightCount] = _pointLight;

						m_Lighting._pointLightCount++;

						break;
					}
					default:
						break;
				}
			}
		}

		return &m_Lighting;
	}

	void Light::UpdateLightTexture()
	{
		ImageDesc _imgDesc;
		TextureRegion _region;

		m_RenderSystem->WriteTexture(*m_LightTexture, _region, _imgDesc);
	}

	void Light::ExcutePass()
	{
		// Light 별로 Shadow Map를 그리는 패스

		m_CommandBuffer->BeginEvent(TEXT("Shadow Map Pass"));

		m_CommandBuffer->BeginEvent(TEXT("DirectionalLight Shadow Pass"));

		for (uint32 _idx = 0; _idx < m_Lighting._dirLightCount; _idx++)
		{
			Culling(m_Lighting._directionLight[_idx]);

			{
				m_CascadedShadow_Pass->BeginExcute(m_CommandBuffer, nullptr);

				m_CascadedShadow_Pass->Excute(m_CommandBuffer);

				m_CascadedShadow_Pass->EndExcute(m_CommandBuffer);
			}

			{
				m_CascadedShadow_Skinned_Pass->BeginExcute(m_CommandBuffer, nullptr);

				m_CascadedShadow_Skinned_Pass->Excute(m_CommandBuffer);

				m_CascadedShadow_Skinned_Pass->EndExcute(m_CommandBuffer);
			}

		}

		m_CommandBuffer->EndEvent();

		m_CommandBuffer->BeginEvent(TEXT("PointLight Shadow Pass"));

		for (uint32 _idx = 0; _idx < m_Lighting._pointLightCount; _idx++)
		{
			m_PointShadow_Pass->SetRenderTarget(m_PointShadowRenderTarget[_idx]);
			m_PointShadow_Skinned_Pass->SetRenderTarget(m_PointShadowRenderTarget[_idx]);

			m_PointShadow_Pass->ClearRenderObject();
			m_PointShadow_Skinned_Pass->ClearRenderObject();

			Culling(m_Lighting._pointLight[_idx]);

			m_PointShadow_Pass->UpdatePerFrame(m_CommandBuffer, &m_Lighting._pointLight[_idx], sizeof(PointLight));
			m_PointShadow_Skinned_Pass->UpdatePerFrame(m_CommandBuffer, &m_Lighting._pointLight[_idx], sizeof(PointLight));


			{
				m_PointShadow_Pass->BeginExcute(m_CommandBuffer, nullptr);

				m_PointShadow_Pass->Excute(m_CommandBuffer);

				m_PointShadow_Pass->EndExcute(m_CommandBuffer);
			}

			{
				m_PointShadow_Skinned_Pass->BeginExcute(m_CommandBuffer, nullptr);

				m_PointShadow_Skinned_Pass->Excute(m_CommandBuffer);

				m_PointShadow_Skinned_Pass->EndExcute(m_CommandBuffer);
			}
		}

		m_CommandBuffer->EndEvent();

		m_CommandBuffer->BeginEvent(TEXT("SpotLight Shadow Pass"));

		for (uint32 _idx = 0; _idx < m_Lighting._spotLightCount; _idx++)
		{
			m_SpotShadow_Pass->SetRenderTarget(m_SpotShadowRenderTarget[_idx]);
			m_SpotShadow_Skinned_Pass->SetRenderTarget(m_SpotShadowRenderTarget[_idx]);

			m_SpotShadow_Pass->ClearRenderObject();
			m_SpotShadow_Skinned_Pass->ClearRenderObject();

			Culling(m_Lighting._spotLight[_idx]);

			m_SpotShadow_Pass->UpdatePerFrame(m_CommandBuffer, &m_Lighting._spotLight[_idx]._lightTransform, sizeof(m_Lighting._spotLight[_idx]._lightTransform));
			m_SpotShadow_Skinned_Pass->UpdatePerFrame(m_CommandBuffer, &m_Lighting._spotLight[_idx]._lightTransform, sizeof(m_Lighting._spotLight[_idx]._lightTransform));

			{
				m_SpotShadow_Pass->BeginExcute(m_CommandBuffer, nullptr);

				m_SpotShadow_Pass->Excute(m_CommandBuffer);

				m_SpotShadow_Pass->EndExcute(m_CommandBuffer);
			}

			{
				m_SpotShadow_Skinned_Pass->BeginExcute(m_CommandBuffer, nullptr);

				m_SpotShadow_Skinned_Pass->Excute(m_CommandBuffer);

				m_SpotShadow_Skinned_Pass->EndExcute(m_CommandBuffer);
			}
		}

		m_CommandBuffer->EndEvent();

		m_CommandBuffer->EndEvent(); // ShadowMap Pass

		m_StaticRenderObjectList.clear();
		m_SkinnedRenderObjectList.clear();
	}

	void Light::RegistStaticRenderObject(RenderObject& renderObject)
	{
		m_StaticRenderObjectList.push_back(renderObject);
	}

	void Light::RegistSkinnedRenderObject(RenderObject& renderObject)
	{
		m_SkinnedRenderObjectList.push_back(renderObject);
	}

	void Light::Init()
	{
		m_LightTexture = m_ResourceManager->GetTexture(TEXT("LightTexture"));
		m_PointLightShadowTexture = m_ResourceManager->GetTexture(TEXT("PointLightShadow"));
		m_SpotLightShadowTexture = m_ResourceManager->GetTexture(TEXT("SpotLightShadow"));

		m_CascadedShadow_Pass = m_ResourceManager->GetRenderPass(TEXT("CascadedShadow Pass"));
		m_CascadedShadow_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("CascadedShadow_Skinned Pass"));

		m_PointShadow_Pass = m_ResourceManager->GetRenderPass(TEXT("PointLightShadow Pass"));
		m_PointShadow_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("PointLightShadow_Skinned Pass"));

		m_SpotShadow_Pass = m_ResourceManager->GetRenderPass(TEXT("SpotLightShadow Pass"));
		m_SpotShadow_Skinned_Pass = m_ResourceManager->GetRenderPass(TEXT("SpotLightShadow_Skinned Pass"));


		std::vector<AttachmentClear> _attachmentClears =
		{
			{ 1, 0 }
		};

		m_CascadedShadow_Pass->SetAttachmentClears(_attachmentClears);

		m_PointShadow_Pass->SetAttachmentClears(_attachmentClears);

		m_SpotShadow_Pass->SetAttachmentClears(_attachmentClears);

		CreateRenderTarget();
	}

	void Light::Culling(PointLight& pointLight)
	{
		// PointLight Culling

		// BoundingSphere와, 렌더 오브젝트의 BoundingOrientedBox가 충돌 하면 랜더 패스에 그려야하는 오브젝트로 등록

		BoundingSphere _boundSphere(pointLight._position, pointLight._range);

		for (size_t i = 0; i < m_StaticRenderObjectList.size(); i++)
		{
			auto _min = m_StaticRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMin();
			auto _max = m_StaticRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMax();

			auto _center = (_min + _max) * 0.5f;
			auto _extents = _max - _center;

			BoundingOrientedBox _boundingOrientedBox;
			_boundingOrientedBox.Center = _center;
			_boundingOrientedBox.Extents = _extents;

			_boundingOrientedBox.Transform(_boundingOrientedBox, m_StaticRenderObjectList[i].m_World);

			if (_boundSphere.Intersects(_boundingOrientedBox))
			{
				m_PointShadow_Pass->RegistRenderObject(&m_StaticRenderObjectList[i]);
			}
		}

		for (size_t i = 0; i < m_SkinnedRenderObjectList.size(); i++)
		{
			auto _min = m_SkinnedRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMin();
			auto _max = m_SkinnedRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMax();

			auto _center = (_min + _max) * 0.5f;
			auto _extents = _max - _center;

			BoundingOrientedBox _boundingOrientedBox;
			_boundingOrientedBox.Center = _center;
			_boundingOrientedBox.Extents = _extents;

			_boundingOrientedBox.Transform(_boundingOrientedBox, m_SkinnedRenderObjectList[i].m_World);

			if (_boundSphere.Intersects(_boundingOrientedBox))
			{
				m_PointShadow_Skinned_Pass->RegistRenderObject(&m_SkinnedRenderObjectList[i]);
			}
		}
	}

	void Light::Culling(SpotLight& spotLight)
	{
		// SpotLight Culling

		// BoundingFrustum과, 렌더 오브젝트의 BoundingOrientedBox가 충돌 하면 랜더 패스에 그려야하는 오브젝트로 등록

		Math::Matrix _spotProj = Math::Matrix::CreatePerspectiveFieldOfView(spotLight._spotAngle, 1.0f, 0.1f, spotLight._range);
		
		Math::Matrix _spotWorld = Math::Matrix::CreateWorld(spotLight._position, spotLight._direction, Math::Vector3::Up);

		BoundingFrustum _boundingFrustum{ _spotProj };

		_boundingFrustum.Transform(_boundingFrustum, _spotWorld);

		for (size_t i = 0; i < m_StaticRenderObjectList.size(); i++)
		{
			auto _min = m_StaticRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMin();
			auto _max = m_StaticRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMax();

			auto _center = (_min + _max) * 0.5f;
			auto _extents = _max - _center;

			BoundingOrientedBox _boundingOrientedBox;
			_boundingOrientedBox.Center = _center;
			_boundingOrientedBox.Extents = _extents;

			_boundingOrientedBox.Transform(_boundingOrientedBox, m_StaticRenderObjectList[i].m_World);

			if (_boundingFrustum.Intersects(_boundingOrientedBox))
			{
				m_SpotShadow_Pass->RegistRenderObject(&m_StaticRenderObjectList[i]);
			}
		}

		for (size_t i = 0; i < m_SkinnedRenderObjectList.size(); i++)
		{
			auto _min = m_SkinnedRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMin();
			auto _max = m_SkinnedRenderObjectList[i].m_MeshBuffer->GetBoundingBoxMax();

			auto _center = (_min + _max) * 0.5f;
			auto _extents = _max - _center;

			BoundingOrientedBox _boundingOrientedBox;
			_boundingOrientedBox.Center = _center;
			_boundingOrientedBox.Extents = _extents;

			_boundingOrientedBox.Transform(_boundingOrientedBox, m_SkinnedRenderObjectList[i].m_World);

			if (_boundingFrustum.Intersects(_boundingOrientedBox))
			{
				m_SpotShadow_Pass->RegistRenderObject(&m_SkinnedRenderObjectList[i]);
			}
		}
	}

	void Light::Culling(DirectionLight& pointLight)
	{
		// DirectionLight Culling
		
		// 직사광의 경우 거의 모든 오브젝트가 그려져야 함으로 컬링 처리가 필요한가?
		// 그냥 직사광을 씬당 하나로 제한하는 것이 맞지 않나?

		for (auto& _renderObject : m_StaticRenderObjectList)
		{
			m_CascadedShadow_Pass->RegistRenderObject(&_renderObject);
		}

		for (auto& _renderObject : m_SkinnedRenderObjectList)
		{
			m_CascadedShadow_Skinned_Pass->RegistRenderObject(&_renderObject);
		}
	}

	void Light::CreateRenderTarget()
	{
		// 라이트의 종류에 따른 랜더 타겟 생성 필요
		// 스팟 라이트는 하나만
		// 포인트 라이트는 랜더 타겟을 6개의 면을 하나의 랜더 타겟으로 생성 필요

		for (uint32 i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			uuid _uuid = TEXT("POINTLIGHT_SHADOWMAP_RT_") + std::to_wstring(i);

			RenderTargetDesc _renderTargetDesc;

			_renderTargetDesc._extend = { 512, 512 };
			_renderTargetDesc._sample = 1;

			AttachmentDesc _depthAttachDesc;

			_depthAttachDesc._renderTargetType = RenderTargetType::DepthStencil;
			_depthAttachDesc._resource = m_PointLightShadowTexture;
			_depthAttachDesc._mipLevel = 0;
			_depthAttachDesc._arrayLayer = i * 6;
			_depthAttachDesc._arraySize = 6;

			_renderTargetDesc._attachments.push_back(_depthAttachDesc);

			auto* _rt = m_ResourceManager->CreateRenderTarget(_uuid, _renderTargetDesc);

			m_PointShadowRenderTarget.push_back(_rt);
		}

		for (uint32 i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			uuid _uuid = TEXT("SPOTLIGHT_SHADOWMAP_RT_") + std::to_wstring(i);

			RenderTargetDesc _renderTargetDesc;

			_renderTargetDesc._extend = { 512, 512 };
			_renderTargetDesc._sample = 1;

			AttachmentDesc _depthAttachDesc;

			_depthAttachDesc._renderTargetType = RenderTargetType::DepthStencil;
			_depthAttachDesc._resource = m_SpotLightShadowTexture;
			_depthAttachDesc._mipLevel = 0;
			_depthAttachDesc._arrayLayer = i;
			_depthAttachDesc._arraySize = 1;

			_renderTargetDesc._attachments.push_back(_depthAttachDesc);

			auto* _rt = m_ResourceManager->CreateRenderTarget(_uuid, _renderTargetDesc);

			m_SpotShadowRenderTarget.push_back(_rt);
		}
	}

}