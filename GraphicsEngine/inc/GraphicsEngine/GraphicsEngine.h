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
	class CameraBuffer;
	class LightBuffer;

	class ResourceManager;

	class IBL;

	class GRAPHICSENGINE_DLL_DECLSPEC GraphicsEngine
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& desc);
		~GraphicsEngine();

		void Initialize(const GraphicsEngineDesc& desc);

		void InitMeshPass();
		void InitLightPass();
		void InitSkyBoxPass();
		void InitDebugPass();

		void InitIBL();
		void CreateIBL();

		MeshBuffer* CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs);
		MaterialBuffer* CreateMaterialBuffer(uuid uuid);
		CameraBuffer* CreateCameraBuffer();
		LightBuffer* CreateLightBuffer();

		Texture* LoadTexture(uuid uuid, ImageDesc* imageDesc);

		void OnResize(uint32 _width, uint32 _height);

		void RegistRenderObject(RenderObject& renderObject);

		void Excute();
		void ExcuteRenderPass(Graphics::RenderPass* renderPass);
		void Present();
		Graphics::RenderPass* GetMeshPass() { return m_Deferred_Mesh_Pass.get(); }
		Graphics::RenderPass* GetMeshBumpPass() { return m_Deferred_Mesh_Bump_Pass.get(); }
		Graphics::RenderPass* GetMeshBumpMRAPass() { return m_Deferred_Mesh_Bump_MRA_Pass.get(); }
		Graphics::RenderPass* GetSkinnedMeshPass() { return m_Deferred_Mesh_Skinned_Pass.get(); }
		Graphics::RenderPass* GetSkinnedMeshBumpPass() { return m_Deferred_Mesh_Skinned_Bump_Pass.get(); }
		Graphics::RenderPass* GetSkinnedMeshBumpMRAPass() { return m_Deferred_Mesh_Skinned_Bump_MRA_Pass.get(); }

		Graphics::RenderPass* GetRenderPass(uuid uuid);

	private:
		void GetLightingData(struct Lighting& perLightFrame);

		void LoadGraphicsTable();

		void LoadDllAndCreateRenderSystem();
		void FreeDllAndReleaseRenderSystem();
		void* LoadProcedure(const char* procedureName);

		class RenderSystem* m_RenderSystem;
		Graphics::SwapChain* m_SwapChain;
		Graphics::CommandBuffer* m_CommandBuffer;

		ResourceManager* m_ResourceManager;

		CameraBuffer* m_MainCameraBuffer;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Bump_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Bump_MRA_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Bump_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Bump_MRA_Pass;

		MaterialBuffer* m_Deferred_Light_Material;
		MeshBuffer* m_Screen_Mesh;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Light_Pass;

		MaterialBuffer* m_SkyBox_Material;
		MeshBuffer* m_SkyBox_Mesh;
		std::shared_ptr<Graphics::RenderPass> m_SkyBox_Pass;

		std::vector<std::shared_ptr<LightBuffer>> m_LightBuffers;

		MaterialBuffer* m_Debug_Material;
		std::shared_ptr<Graphics::RenderPass> m_Debug_Pass;

		// Deferrd resource for debug
		Texture* m_Albedo;
		Texture* m_Normal;
		Texture* m_Depth;
		Texture* m_World;

		std::vector<RenderObject> m_DebugRenderObject;

		std::shared_ptr<IBL> m_IBL;
	};

	extern "C"
	{
		GRAPHICSENGINE_DLL_DECLSPEC GraphicsEngine* CreateGraphicsEngine(GraphicsEngineDesc& desc);
		GRAPHICSENGINE_DLL_DECLSPEC void ReleaseGraphicsEngine(GraphicsEngine* graphicsEngine);
	}
}