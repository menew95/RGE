/**

    @file      RenderObject.h
    @brief     ���� ���� ���� ��ü��
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
		@brief ���ҽ����� ������Ʈ �� ������ ���� ��ü
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
		@brief Render component�� �ϳ��� ���� ������Ʈ�� ������ ���� ������ �������� �������� �߰� ��ü
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

		// Ȱ��ȭ�Ǿ��ִ��� üũ
		bool m_bIsEnable = false;
		bool m_IsCulling = false;

		// ������ ������Ʈ�ΰ�
		bool m_bIsStatic = false;

		// �׸��ڸ� �׸��� ������Ʈ�ΰ�
		bool m_bIsCastShadow = false;


		// �ø��� �ϱ�����
		struct TransformMatrix
		{
			Math::Matrix _world;
			Math::Matrix _worldInv;
		};
		TransformMatrix m_TransformMatrix;

		// ��Ű�� ������Ʈ�ΰ�
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
		 @brief RenderData RenderObject���� ��Ƽ���� �ε����� ����Ž� �ε����� ������ ����ִ� ��ü
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
		@brief MeshInstanceData �Ž� �ϵ���� �ν��Ͻ��� �ϱ����� ����ü
	**/
	struct MeshInstanceData
	{
		MeshBuffer* _meshBuffer = nullptr;

		std::vector<RenderData> _renderObjects;
	};

	/**
		 @brief MaterialInstanceData ������ �Ҷ� ������ �������� �ִ��� �ǵ��̱Ⱑ ���� �߰��� ��ü
		 ��Ƽ���� ���� �ν��Ͻ� �����͸� ����ְ� ������ �ϱ� ���� �߰��ߴ�.
		 ���� ��Ƽ���� ���ۿ� �ִ� �����͸� ������Ʈ �Ѵ��� ���ε� �ϰ�
		 ���� ��ü�� ���� �Ž�����, perObject�� ���ε��ϰ� ��ο� �ϱ� ���� �߰���
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

