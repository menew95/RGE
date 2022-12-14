#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "GraphicsModule/Core/Buffer.h"

namespace Graphics
{
	class SubMeshBuffer
	{
	public:
		SubMeshBuffer() = default;
		~SubMeshBuffer() = default;

		void Release(RenderSystem* renderSystem);

		uint32 m_IndexCount;
		Graphics::Buffer* m_IndexBuffer;
	};

	class MeshBuffer : public ResourceBuffer
	{
	public:
		MeshBuffer(RenderSystem* renderSystem);
		virtual ~MeshBuffer();

		void CreateVertexBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices);
		void CreateSubMesh(uuid uuid, std::vector<uint32>& indices);

		inline uint32 GetSubMeshCount() const
		{
			return static_cast<uint32>(m_SubMeshBuffers.size()); 
		}

		inline const Graphics::Buffer& GetBuffer() const
		{
			return *m_VertexBuffer;
		}

		inline const SubMeshBuffer& GetSubMesh(uint32 index) const
		{
			return m_SubMeshBuffers[index];
		}

	private:
		Graphics::Buffer* m_VertexBuffer;
		std::vector<SubMeshBuffer> m_SubMeshBuffers;
	};
}