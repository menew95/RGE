#pragma once

#include "Common.h"

#include "GraphicsEngine/GraphicsEngineFlags.h"

namespace Graphics
{
	class MeshBuffer;
	class MaterialBuffer;
	class ResourceManager;

	class GraphicsEngine
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& desc);
		~GraphicsEngine();

		MeshBuffer* CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs);
		MaterialBuffer* CreateMaterialBuffer(uuid uuid, PipelineLayout* pipelineLayout);

	private:
		void LoadDllAndCreateRenderSystem();
		void FreeDllAndReleaseRenderSystem();
		void* LoadProcedure(const char* procedureName);

		class RenderSystem* m_RenderSystem;
		std::shared_ptr<ResourceManager> m_ResourceManager;
	};
}