#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/InstanceBuffer.h"
#include "GraphicsModule/Core/RenderSystem.h"
#include "GraphicsModule/Core/CommandBuffer.h"

namespace Graphics
{

	InstanceBuffer::InstanceBuffer(Graphics::RenderSystem* renderSystem, uuid uuid, uint32 size)
		: ByteBuffer(size, uuid)
		, m_InstanceBuffer(nullptr)
		, m_BufferSize(0)
	{
		m_RenderSystem = renderSystem;

		ResizeBuffer(size);
	}

	InstanceBuffer::~InstanceBuffer()
	{

	}

	void InstanceBuffer::SetInstanceData(void* src, void* matsrc, uint32 offsetID)
	{
		const uint32 _staticSize = sizeof(Matrix) * 2 + sizeof(Vector4) * 3;

		byte _instance[_staticSize];

		memcpy(_instance, src, sizeof(Matrix) * 2);
		memcpy(_instance + sizeof(Matrix) * 2, matsrc, sizeof(Vector4) * 3);

		UpdateBuffer(_instance, _staticSize, offsetID * _staticSize);
	}

	void InstanceBuffer::SetInstanceData(void* src, void* skin, void* matsrc, uint32 offsetID)
	{
		const uint32 _skinSize = sizeof(Matrix) * 2 + sizeof(Vector4) * 3 + sizeof(Matrix) * 128;

		byte _instance[_skinSize];

		memcpy(_instance, src, sizeof(Matrix) * 2);
		memcpy(_instance + sizeof(Matrix) * 2, matsrc, sizeof(Vector4) * 3);
		memcpy(_instance + sizeof(Matrix) * 2 + sizeof(Vector4) * 3, skin, sizeof(Matrix) * 128);

		UpdateBuffer(_instance, _skinSize, offsetID * _skinSize);
	}

	void InstanceBuffer::UpdateInstanceBuffer(CommandBuffer* commandBuffer)
	{
		commandBuffer->UpdateBuffer(*m_InstanceBuffer, 0, GetByteBuffer(), GetSize());
	}

	void InstanceBuffer::BindBuffer(CommandBuffer* commandBuffer, uint32 slot, uint32 stageFlags)
	{
		commandBuffer->SetResource(*m_InstanceBuffer, slot, BindFlags::ShaderResource, stageFlags);
	}

	void InstanceBuffer::ResizeBuffer(uint32 size)
	{
		if (m_InstanceBuffer != nullptr) m_RenderSystem->Release(*m_InstanceBuffer);

		BufferDesc _desc;

		_desc._size = size;
		_desc._stride = sizeof(Matrix) * 2 + sizeof(Vector4) * 3;
		_desc._format = Format::UNKNOWN;
		_desc._bindFlags = BindFlags::VertexBuffer;
		_desc._miscFlags = MiscFlags::DynamicUsage;

		m_RenderSystem->CreateBuffer(GetUUID(), _desc);

		m_BufferSize = size;


		//_desc._stride = 16;
		//_desc._format = Format::R32G32B32A32_FLOAT;
		//_desc._bindFlags = BindFlags::ShaderResource;
	}

}