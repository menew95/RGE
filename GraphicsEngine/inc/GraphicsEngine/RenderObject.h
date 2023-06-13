/**

    @file      RenderObject.h
    @brief     렌더 정보 관련 객체들
    @author    LWT
    @date      13.05.2023

**/
#pragma once

#include "Common.h"

#include "GraphicsModule/Core/Resource.h"

#include "GraphicsEngine/Export.h"

namespace Graphics
{
	class MeshBuffer;
	class MaterialBuffer;
	class Buffer;

	enum class eUpdateTime
	{
		PerFrame	= 0,
		PerObject	= 1,
		PerMaterial	= 2,
	};

	/**
		@brief 리소스들을 업데이트 할 정보를 담은 객체
	**/
	struct UpdateResourceData
	{
		inline UpdateResourceData()
		{}

		inline UpdateResourceData(eUpdateTime time, uint32 idx, ResourceType type, void* src, uint32 size)
			: _updateTime(time)
			, _index(idx)
			, _resourceType(type)
			, _dataSrc(src)
			, _datasize(size)
		{}

		inline UpdateResourceData(uint32 idx, ResourceType type, void* src, uint32 size)
			: _updateTime(eUpdateTime::PerMaterial)
			, _index(idx)
			, _resourceType(type)
			, _dataSrc(src)
			, _datasize(size)
		{}

		inline UpdateResourceData(uint32 idx,  void* src)
			: _updateTime(eUpdateTime::PerMaterial)
			, _index(idx)
			, _resourceType(ResourceType::Texture)
			, _dataSrc(src)
		{}

		inline UpdateResourceData(void* src, uint32 size)
			: _index(0)
			, _resourceType(ResourceType::Buffer)
			, _dataSrc(src)
			, _datasize(size)
		{}

		eUpdateTime _updateTime = eUpdateTime::PerMaterial;

		uint32 _index = 0;

		ResourceType _resourceType = ResourceType::Undefined;

		void* _dataSrc = nullptr;

		uint32 _datasize = 0;

		bool operator=(UpdateResourceData& r)
		{
			return (_updateTime == r._updateTime &&
				_index == r._index &&
				_resourceType == r._resourceType &&
				_dataSrc == r._dataSrc &&
				_datasize == r._datasize);
		}
	};

	/**
		@brief Render component당 하나의 렌더 오브젝트를 가지며 게임 엔진과 렌더러를 연결해줄 중간 개체
	**/
	class GRAPHICSENGINE_DLL_DECLSPEC RenderObject
	{
	public:
		RenderObject();

		inline const MeshBuffer* GetMeshBuffer() const
		{
			return m_MeshBuffer;
		}

		/*inline const MaterialBuffer* GetMaterialBuffer() const
		{
			return m_MaterialBuffer;
		}*/

		inline size_t GetMaterialBuffersCount() const
		{
			return m_MaterialBuffers.size();
		}

		inline auto GetMaterialBuffers() const
		{
			return m_MaterialBuffers.data();
		}

		/*inline const std::vector<Graphics::Resource*>& GetResources() const
		{
			return m_Resources;
		}*/

		/*inline const std::vector<Graphics::Buffer*>& GetConstBuffers() const
		{
			return m_ConstBuffers;
		}*/

		/*inline const std::vector<UpdateResourceData>& GetUpdateResourceData() const
		{
			return m_UpdateResources;
		}*/

		inline const std::vector<UpdateResourceData>& GetUpdateResourceDataPerObject() const
		{
			return m_UpdateResourcePerObjects;
		}

		inline const bool IsHasViewport() const
		{
			return m_UseViewport;
		}

		inline void SetViewport(std::vector<Math::Viewport> viewports)
		{
			m_UseViewport = true;
			m_Viewports = viewports;
		}

		inline void AddViewport(Math::Viewport viewport)
		{
			m_UseViewport = true;
			m_Viewports.push_back(viewport);
		}

		inline std::vector<Math::Viewport>& GetViewports()
		{
			return m_Viewports;
		}

		// 활성화되어있는지 체크
		bool m_bIsEnable = false;
		bool m_IsCulling = false;

		// 정적인 오브젝트인가
		bool m_bIsStatic = false;

		// 그림자를 그리는 오브젝트인가
		bool m_bIsCastShadow = false;


		// 컬링을 하기위해
		struct TransformMatrix
		{
			Math::Matrix _world;
			Math::Matrix _worldInv;
		};
		TransformMatrix m_TransformMatrix;

		// 스키닝 오브젝트인가
		bool m_bIsSkinned = false;
		void* m_pSkinnedData = nullptr;

		uint32 m_RenderPassIdx = 0;

		MeshBuffer* m_MeshBuffer = nullptr;

		//MaterialBuffer* m_MaterialBuffer = nullptr;

		std::vector<MaterialBuffer*> m_MaterialBuffers;

		//std::vector<Graphics::Resource*> m_Resources;

		//std::vector<UpdateResourceData> m_UpdateResources;

		std::vector<UpdateResourceData> m_UpdateResourcePerObjects;

		bool m_UseViewport = false;

		std::vector<Math::Viewport> m_Viewports;
	};

	/**
		@brief RenderPass에서 Excute할 정보들을 담고있는 객체
	**/
	struct RenderData
	{
		Buffer* _vertex = nullptr;
		Buffer* _index = nullptr;

		MeshBuffer* _meshBuffer = nullptr;

		MaterialBuffer* _materialBuffer = nullptr;

		std::vector<UpdateResourceData> _updateResourcePerObjects;

		bool _useViewport = false;

		std::vector<Math::Viewport> _viewports;
	};
}

