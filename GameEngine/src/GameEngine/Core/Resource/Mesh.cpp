#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/Resource/Mesh.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	registration::class_<GameEngine::Core::Mesh>("Mesh")
	.constructor<const tstring&>();
}


namespace GameEngine
{
	namespace Core
	{

		Mesh::Mesh(const tstring& typeName /*= TEXT("Mesh")*/)
			: Resource(typeName)
		{

		}

		Mesh::Mesh(std::vector<VertexAttribute>& vertices, std::vector<std::vector<uint32>>& indices, const tstring& typeName /*= TEXT("Mesh")*/)
			: Resource(typeName)
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