#pragma once

#include "Common.h"

#include "GraphicsModule/Core/Resource.h"

#include "GraphicsEngine/Export.h"

namespace Graphics
{
	class MeshBuffer;
	class MaterialBuffer;
	class Buffer;

	struct UpdateResourceData
	{
		inline UpdateResourceData()
		{}

		inline UpdateResourceData(uint32 idx, ResourceType type, void* src, uint32 size)
			: _index(idx)
			, _resourceType(type)
			, _dataSrc(src)
			, _datasize(size)
		{}

		inline UpdateResourceData(void* src, uint32 size)
			: _index(0)
			, _resourceType(ResourceType::Buffer)
			, _dataSrc(src)
			, _datasize(size)
		{}


		uint32 _index = 0;

		ResourceType _resourceType = ResourceType::Undefined;

		void* _dataSrc = nullptr;

		uint32 _datasize = 0;
	};

	// 랜더 패스에 하나의 랜더 오브젝트를 정의
	class GRAPHICSENGINE_DLL_DECLSPEC RenderObject
	{
	public:
		RenderObject();

		inline const MeshBuffer* GetMeshBuffer() const
		{
			return m_MeshBuffer;
		}

		inline const MaterialBuffer* GetMaterialBuffer() const
		{
			return m_MaterialBuffer;
		}

		inline const std::vector<Graphics::Resource*>& GetResources() const
		{
			return m_Resources;
		}

		inline const std::vector<Graphics::Buffer*>& GetConstBuffers() const
		{
			return m_ConstBuffers;
		}

		inline const std::vector<UpdateResourceData>& GetUpdateResourceData() const
		{
			return m_UpdateResources;
		}

	//private:
		MeshBuffer* m_MeshBuffer;
		MaterialBuffer* m_MaterialBuffer;

		std::vector<Graphics::Resource*> m_Resources;

		std::vector<Graphics::Buffer*> m_ConstBuffers;

		std::vector<UpdateResourceData> m_UpdateResources;

		UpdateResourceData m_UpdateResourcePerDraw;
	};
}

