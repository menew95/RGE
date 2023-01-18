#pragma once

#include "Common.h"

#include "GraphicsEngine/Export.h"

#include "GraphicsEngine/GraphicsEngineFlags.h"

#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

namespace Graphics
{
	class MeshBuffer;
	class MaterialBuffer;
	class ResourceManager;

	class GRAPHICSENGINE_DLL_DECLSPEC GraphicsEngine
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& desc);
		~GraphicsEngine();

		void Initialize(const GraphicsEngineDesc& desc);

		void IntiLightPass();

		MeshBuffer* CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs);
		MaterialBuffer* CreateMaterialBuffer(uuid uuid, PipelineLayout* pipelineLayout);

		Texture* LoadTexture(uuid uuid, ImageDesc* imageDesc);

		void OnResize(uint32 _width, uint32 _height);

		void RegistRenderObject(RenderObject& renderObject);

		void Excute();

	private:
		void LoadGraphicsTable();

		void LoadDllAndCreateRenderSystem();
		void FreeDllAndReleaseRenderSystem();
		void* LoadProcedure(const char* procedureName);

		class RenderSystem* m_RenderSystem;
		Graphics::SwapChain* m_SwapChain;
		Graphics::CommandBuffer* m_CommandBuffer;

		std::shared_ptr<ResourceManager> m_ResourceManager;

		Graphics::RenderPass* m_Deferred_Mesh_Pass;
		Graphics::RenderPass* m_Deferred_Light_Pass;

		MaterialBuffer* m_Deferred_Light_Material;
		MeshBuffer* m_Screen_Mesh;
	};

	extern "C"
	{
		GRAPHICSENGINE_DLL_DECLSPEC GraphicsEngine* CreateGraphicsEngine(GraphicsEngineDesc& desc);
		GRAPHICSENGINE_DLL_DECLSPEC void ReleaseGraphicsEngine(GraphicsEngine* graphicsEngine);
	}
}