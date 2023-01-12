#pragma once

#include "Common.h"


namespace Graphics
{
	class RenderSystem;

	class MeshBuffer;
	class MaterialBuffer;

	class ResourceManager
	{
	public:
		ResourceManager(Graphics::RenderSystem* renderSystem);
		~ResourceManager();

		MeshBuffer* CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs);
		MaterialBuffer* CreateMaterialBuffer(uuid uuid, Graphics::PipelineLayout* pipelineLayout);
	
	private:
		Graphics::RenderSystem* m_RenderSystem;

		std::map<uuid, MeshBuffer*> m_MeshBuffers;
		std::map<uuid, MaterialBuffer*> m_MaterialBuffers;
	};
}