#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderQueue/RenderQueue.h"

#include "GraphicsEngine/RenderPass/Deferred.h"
#include "GraphicsEngine/RenderPass/Light.h"
#include "GraphicsEngine/RenderPass/Sky.h"

namespace Graphics
{
	RenderQueue::RenderQueue()
	{

	}

	RenderQueue::~RenderQueue()
	{

	}

	Graphics::RenderObject* RenderQueue::CreateRenderObject()
	{
		auto _obj = std::make_shared<RenderObject>();

		m_RenderObjectContainer.emplace_back(_obj);

		return m_RenderObjectContainer.back().get();
	}

	bool RenderQueue::RemoveRenderObject(RenderObject* renderObject)
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

	void RenderQueue::RegistDeferredPass(Deferred* deferred)
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

	void RenderQueue::RegistShadowPass(Light* light)
	{
		for (auto& _renderObject : m_RenderObjectContainer)
		{
			if (!_renderObject->m_bIsEnable) continue;

			if (!_renderObject->m_bIsCastShadow) continue;

			_renderObject->m_UpdateResourcePerObjects.clear();

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

				_renderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);

				light->RegistStaticRenderObject(_renderObject.get());
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

				_renderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);

				Graphics::UpdateResourceData _perSkinnedObjectResource
				{
					_perSkinnedObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
					_perSkinnedObjectResource._index = 2,
					_perSkinnedObjectResource._resourceType = Graphics::ResourceType::Buffer,
					_perSkinnedObjectResource._dataSrc = _renderObject->m_pSkinnedData,
					_perSkinnedObjectResource._datasize = sizeof(Math::Matrix) * 128
				};

				_renderObject->m_UpdateResourcePerObjects.emplace_back(_perSkinnedObjectResource);

				light->RegistSkinnedRenderObject(_renderObject.get());
			}

		}
	}

	void RenderQueue::RegistVoxelPass(Voxel* voxel)
	{
		for (auto& _renderObject : m_RenderObjectContainer)
		{
			if (!_renderObject->m_bIsEnable) continue;

			_renderObject->m_UpdateResourcePerObjects.clear();

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

				_renderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);

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

				_renderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);

				Graphics::UpdateResourceData _perSkinnedObjectResource
				{
					_perSkinnedObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
					_perSkinnedObjectResource._index = 2,
					_perSkinnedObjectResource._resourceType = Graphics::ResourceType::Buffer,
					_perSkinnedObjectResource._dataSrc = _renderObject->m_pSkinnedData,
					_perSkinnedObjectResource._datasize = sizeof(Math::Matrix) * 128
				};

				_renderObject->m_UpdateResourcePerObjects.emplace_back(_perSkinnedObjectResource);

				
			}
		}
	}

}