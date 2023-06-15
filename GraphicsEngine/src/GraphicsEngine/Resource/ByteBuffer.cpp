#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/ByteBuffer.h"

namespace Graphics
{

	ByteBuffer::ByteBuffer(uint32 bufferSize, uuid uuid)
		: ResourceBuffer(BufferType::Byte, uuid)
		, m_BufferSize(bufferSize)
	{
		m_ByteBuffer = new byte[bufferSize];
	}

	ByteBuffer::~ByteBuffer()
	{
		delete[] m_ByteBuffer;
	}


	void ByteBuffer::UpdateBuffer(void* src, uint32 srcSize, uint32 startPoint /*= 0*/)
	{
		if (src == nullptr) return;

		if (srcSize + startPoint >= m_BufferSize)
		{
			// �ִ� ����� ������ ��� ����ũ�⸦ �÷���
			ResizeBuffer(m_BufferSize - startPoint + srcSize);
		}

		memcpy(m_ByteBuffer + startPoint, src, srcSize);

		//memcpy_s(m_ByteBuffer + startPoint, m_BufferSize - startPoint, src, srcSize);
	}

	void ByteBuffer::ResizeBuffer(uint32 size)
	{
		delete[] m_ByteBuffer;

		m_ByteBuffer = new byte[size];

		m_BufferSize = size;
	}

}