#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/InstanceBuffer.h"

namespace Graphics
{

	InstanceBuffer::InstanceBuffer(Graphics::RenderSystem* renderSystem, uuid uuid)
		: ByteBuffer(5000, uuid)
		, m_InstanceBuffer(nullptr)
	{

	}

	InstanceBuffer::~InstanceBuffer()
	{

	}

	void InstanceBuffer::SetInstanceData(void* src, void* matsrc, uint32 offsetID)
	{
		const uint32 _staticSize = sizeof(Matrix) * 2 + 48;

		byte _instance[_staticSize];

		memcpy(_instance, src, sizeof(Matrix) * 2);
		memcpy(_instance + sizeof(Matrix) * 2, matsrc, sizeof(float) * 12);

		UpdateBuffer(_instance, _staticSize, offsetID * _staticSize);
	}

	void InstanceBuffer::SetInstanceData(void* src, void* skin, void* matsrc, uint32 offsetID)
	{
		const uint32 _skinSize = sizeof(Matrix) * 2 + 48 + sizeof(Matrix) * 128;

		byte _instance[_skinSize];

		memcpy(_instance, src, sizeof(Matrix) * 2);
		memcpy(_instance + sizeof(Matrix) * 2, matsrc, sizeof(float) * 12);
		memcpy(_instance + sizeof(Matrix) * 2 + sizeof(float) * 12, skin, sizeof(Matrix) * 128);

		UpdateBuffer(_instance, _skinSize, offsetID * _skinSize);
	}

}