#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"

namespace Graphics
{
	MeshBuffer::MeshBuffer(RenderSystem* renderSystem)
		: ResourceBuffer(renderSystem, BufferType::Mesh)
		, m_VertexBuffer(nullptr)
	{

	}

	MeshBuffer::~MeshBuffer()
	{
		m_RenderSystem->Release(*m_VertexBuffer);
		m_VertexBuffer = nullptr;

		for (auto& _subMesh : m_SubMeshBuffers)
		{
			_subMesh.Release(m_RenderSystem);
		}
	}

	void MeshBuffer::CreateVertexBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices)
	{
		BufferDesc _bufferDesc;
		_bufferDesc._bindFlags = BindFlags::VertexBuffer;
		_bufferDesc._miscFlags = MiscFlags::DynamicUsage;
		_bufferDesc._size = static_cast<uint32>(sizeof(Common::VertexAttribute) * vertices.size());

		m_VertexBuffer = m_RenderSystem->CreateBuffer(uuid, _bufferDesc, vertices.data());
	}

	void MeshBuffer::CreateSubMesh(uuid uuid, std::vector<uint32>& indices)
	{
		SubMeshBuffer _subMeshBuffer;

		BufferDesc _bufferDesc;
		_bufferDesc._bindFlags = BindFlags::IndexBuffer;
		_bufferDesc._miscFlags = MiscFlags::DynamicUsage;
		_bufferDesc._size = static_cast<uint32>(sizeof(uint32) * indices.size());

		_subMeshBuffer.m_IndexBuffer = m_RenderSystem->CreateBuffer(uuid, _bufferDesc, indices.data());
		_subMeshBuffer.m_IndexCount = static_cast<uint32>(indices.size());
	}

	void SubMeshBuffer::Release(RenderSystem* renderSystem)
	{
		renderSystem->Release(*m_IndexBuffer);
		m_IndexBuffer = nullptr;
	}

}