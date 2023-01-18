#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/Resource/Mesh.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

namespace GameEngine
{
	namespace Core
	{

		Mesh::Mesh(const tstring& typeName /*= TEXT("Mesh")*/)
		{

		}

		Mesh::Mesh(std::vector<VertexAttribute>& vertices, std::vector<std::vector<uint32>>& indices, const tstring& typeName /*= TEXT("Mesh")*/)
		{
			m_VertexAttributes = vertices;
			m_IndexAttributers = indices;
		}

		Mesh::~Mesh()
		{
			Release();
		}

		void Mesh::Release()
		{
			GraphicsSystem::GetInstance()->DeleteMeshBuffer(m_MeshBuffer);
		}
	}
}