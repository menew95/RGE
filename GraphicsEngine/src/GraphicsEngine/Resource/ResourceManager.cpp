#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/ResourceManager.h"

#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"

namespace Graphics
{

	ResourceManager::ResourceManager(Graphics::RenderSystem* renderSystem)
		: m_RenderSystem(renderSystem)
	{

	}

	ResourceManager::~ResourceManager()
	{

	}

	Graphics::MeshBuffer* ResourceManager::CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs)
	{
		//auto iter = std::find_if(std::begin(m_MeshBuffers), std::end(m_MeshBuffers),
		//		[&uuid](auto& _pair) = > {
		//		return (_pair.first == uuid)
		//	}
		//);

		return nullptr;
	}

	Graphics::MaterialBuffer* ResourceManager::CreateMaterialBuffer(uuid uuid, Graphics::PipelineLayout* pipelineLayout)
	{
		return nullptr;
	}

}