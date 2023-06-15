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

		inline size_t GetMaterialBuffersCount() const
		{
			return m_MaterialBuffers.size();
		}

		inline auto GetMaterialBuffers() const
		{
			return m_MaterialBuffers.data();
		}

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

		std::vector<UpdateResourceData> m_UpdateResourcePerObjects;

		bool m_UseViewport = false;

		std::vector<Math::Viewport> m_Viewports;
	};

	 /**
		 @brief RenderData RenderObject에서 머티리얼 인덱스와 서브매쉬 인덱스의 정보를 담고있는 객체
	 **/
	struct RenderData
	{
		RenderData()
			: _renderObject(nullptr)
			, _materialIdx(-1)
			, _subMeshIdx(-1)
		{

		}

		RenderData(RenderObject* renderObject, int32 materialIdx)
			: _renderObject(renderObject)
			, _materialIdx(materialIdx)
			, _subMeshIdx(materialIdx)
		{

		}

		RenderData(RenderObject* renderObject, int32 materialIdx, int32 subMeshIdx)
			: _renderObject(renderObject)
			, _materialIdx(materialIdx)
			, _subMeshIdx(subMeshIdx)
		{

		}

		RenderObject* _renderObject = nullptr;

		int32 _materialIdx = -1;
		int32 _subMeshIdx = -1;
	};

	/**
		@brief MeshInstanceData 매쉬 하드웨어 인스턴싱을 하기위한 구조체
	**/
	struct MeshInstanceData
	{
		MeshBuffer* _meshBuffer = nullptr;

		std::vector<RenderData> _renderObjects;
	};

	/**
		 @brief MaterialInstanceData 렌더링 할때 도무지 기존것을 최대한 건들이기가 힘들어서 추가한 객체
		 머티리얼 별로 인스턴스 데이터를 모아주고 렌더링 하기 위해 추가했다.
		 먼저 머티리얼 버퍼에 있는 데이터를 업데이트 한다음 바인드 하고
		 렌더 객체를 돌며 매쉬버퍼, perObject를 바인드하고 드로우 하기 위해 추가함
	 **/
	struct MaterialInstanceData
	{
		MaterialBuffer* _materialBuffer = nullptr;

		std::vector<MeshInstanceData> _meshInstanceData;
	};

	struct InstanceData
	{
		MaterialInstanceData _materialInstanceData;

		MeshInstanceData _meshInstnaceData;
	};
}

