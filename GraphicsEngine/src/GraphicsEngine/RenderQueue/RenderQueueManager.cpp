#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderQueue/RenderQueueManager.h"
#include "GraphicsEngine/RenderQueue/RenderQueue.h"

#include "GraphicsEngine/RenderPass/Deferred.h"
#include "GraphicsEngine/RenderPass/Light.h"
#include "GraphicsEngine/RenderPass/Sky.h"

#include "GraphicsEngine/Culling/Frustum.h"

#include "GraphicsEngine/Resource/CameraBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"

namespace Graphics
{
	SINGTON_IMPL(Graphics::RenderQueueManager)

	RenderQueueManager::RenderQueueManager()
	{

	}

	RenderQueueManager::~RenderQueueManager()
	{

	}

	Graphics::RenderObject* RenderQueueManager::CreateRenderObject()
	{
		auto _obj = std::make_shared<RenderObject>();

		m_RenderObjectContainer.emplace_back(_obj);

		return m_RenderObjectContainer.back().get();
	}

	bool RenderQueueManager::RemoveRenderObject(RenderObject* renderObject)
	{
		auto _iter = std::find_if(m_RenderObjectContainer.begin(), m_RenderObjectContainer.end(), 
			[&renderObject](std::shared_ptr<RenderObject>& ptr) { return (ptr.get() == renderObject); }
		);

		auto _ptr = std::make_shared<RenderObject>();

		if (_iter != m_RenderObjectContainer.end())
		{
			m_RenderObjectContainer.erase(_iter);

			return true;
		}

		return false;
	}

	void RenderQueueManager::RegistDeferredPass(Deferred* deferred)
	{
		for (auto& _renderObject : m_RenderObjectContainer)
		{
			if (!_renderObject->m_bIsEnable) continue;

			_renderObject->m_UpdateResourcePerObjects.clear();

			uint32 _idx = 0;

			if (!_renderObject->m_bIsSkinned)
			{
				// static mesh
				Graphics::UpdateResourceData _perObjectResource
				{
					_perObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
					_perObjectResource._index = 1,
					_perObjectResource._resourceType = Graphics::ResourceType::Buffer,
					_perObjectResource._dataSrc = &_renderObject->m_TransformMatrix,
					_perObjectResource._datasize = sizeof(Math::Matrix) * 2
				};

				//_renderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);
			}
			else
			{
				// skinned mesh
				Graphics::UpdateResourceData _perObjectResource
				{
					_perObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
					_perObjectResource._index = 1,
					_perObjectResource._resourceType = Graphics::ResourceType::Buffer,
					_perObjectResource._dataSrc = &_renderObject->m_TransformMatrix,
					_perObjectResource._datasize = sizeof(Math::Matrix) * 2
				};

				//_renderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);

				Graphics::UpdateResourceData _perSkinnedObjectResource
				{
					_perSkinnedObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
					_perSkinnedObjectResource._index = 2,
					_perSkinnedObjectResource._resourceType = Graphics::ResourceType::Buffer,
					_perSkinnedObjectResource._dataSrc = _renderObject->m_pSkinnedData,
					_perSkinnedObjectResource._datasize = sizeof(Math::Matrix) * 128
				};

				//_renderObject->m_UpdateResourcePerObjects.emplace_back(_perSkinnedObjectResource);

				_idx += 4;
			}

			_renderObject->m_RenderPassIdx = _idx;

			deferred->RegistRenderObject(_renderObject.get());
		}
	}

	void RenderQueueManager::RegistShadowPass(Light* light)
	{
		for (auto& _renderObject : m_RenderObjectContainer)
		{
			if (!_renderObject->m_bIsEnable) continue;

			if (!_renderObject->m_bIsCastShadow) continue;

			_renderObject->m_UpdateResourcePerObjects.clear();

			if (!_renderObject->m_bIsSkinned)
			{
				// static mesh

				light->RegistStaticRenderObject(_renderObject.get());
			}
			else
			{
				// skinned mesh

				light->RegistSkinnedRenderObject(_renderObject.get());
			}

		}
	}

	void RenderQueueManager::RegistVoxelPass(Voxel* voxel)
	{
		for (auto& _renderObject : m_RenderObjectContainer)
		{
			if (!_renderObject->m_bIsEnable) continue;

			_renderObject->m_UpdateResourcePerObjects.clear();

		}
	}

	void RenderQueueManager::CreateRenderQueue(RenderQueue& renderQueue)
	{
		Frustum _frustum();

		for (auto& _renderObject : m_RenderObjectContainer)
		{
			if (!_renderObject->m_bIsEnable) continue;

			// push shadow draw
			if (_renderObject->m_bIsCastShadow)
			{
				// Todo : 인스턴싱을 어케 처리하지

				if (!_renderObject->m_bIsSkinned)
				{
					// static mesh

					//light->RegistStaticRenderObject(_renderObject.get());
				}
				else
				{
					// skinned mesh

					//light->RegistSkinnedRenderObject(_renderObject.get());
				}
			}

			// push mesh draw
			for (size_t i = 0; i < _renderObject->GetMaterialBuffersCount(); i++)
			{
				RenderData _renderData(_renderObject.get(), static_cast<uint32>(i));

				if(_renderObject->GetMaterialBuffer(i) != nullptr)
					 renderQueue.Push(_renderObject->GetMaterialBuffer(i), _renderData);
			}
		}
	}

	void RenderQueueManager::UpdateRenderQueue(CameraBuffer* camBuffer)
	{
		Frustum _frustum = camBuffer->GetFrustum();

		for (auto& _renderObject : m_RenderObjectContainer)
		{
			if (!_renderObject->m_bIsEnable) continue;

			if(!_frustum.IsIntersects(
				_renderObject->m_TransformMatrix._world,
				_renderObject->GetMeshBuffer()->GetBoundingBoxMin(),
				_renderObject->GetMeshBuffer()->GetBoundingBoxMax()))
				continue;

			// push shadow draw
			if (_renderObject->m_bIsCastShadow)
			{
				// Todo : 인스턴싱을 어케 처리하지

				if (!_renderObject->m_bIsSkinned)
				{
					// static mesh

					//light->RegistStaticRenderObject(_renderObject.get());
				}
				else
				{
					// skinned mesh

					//light->RegistSkinnedRenderObject(_renderObject.get());
				}
			}

			// push mesh draw
			for (size_t i = 0; i < _renderObject->GetMaterialBuffersCount(); i++)
			{
				RenderData _renderData(_renderObject.get(), static_cast<uint32>(i));

				MaterialBuffer* _matBuf = _renderObject->GetMaterialBuffer(i);
				if (_matBuf != nullptr)
				{
					switch (_matBuf->GetRenderMode())
					{
						case RenderMode::Opaque:
						{
							break;
						}
						case RenderMode::Transparent:
						{
							break;
						}
					}
				}

			}
		}
	}

}