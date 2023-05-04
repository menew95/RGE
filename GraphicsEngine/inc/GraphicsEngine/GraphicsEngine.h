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
	class Light;
	class Deferred;
	class RenderQueue;
	class Sky;
	class PostProcess;
	class Voxel;

	class DebugDeferred;

	class GRAPHICSENGINE_DLL_DECLSPEC GraphicsEngine
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& desc);
		~GraphicsEngine();

		void Initialize(const GraphicsEngineDesc& desc);

		void InitMeshPass();
		void InitLightPass();
		void InitSkyPass();
		void InitDebugPass();

		void InitIBL();
		void CreateIBL();

		void InitLight();
		void InitCascadedShadow();

		void InitSSR();
		void InitVoxel();

		MeshBuffer* CreateMeshBuffer(uuid uuid, std::vector<Common::VertexAttribute>& vertices, std::vector<std::vector<uint32>> subMeshs, Math::Vector3 min, Math::Vector3 max);
		MaterialBuffer* CreateMaterialBuffer(uuid uuid);
		CameraBuffer* CreateCameraBuffer();
		LightBuffer* CreateLightBuffer();

		Texture* LoadTexture(uuid uuid, ImageDesc* imageDesc);

		void OnResize(uint32 _width, uint32 _height);

		void RegistRenderObject(RenderObject* renderObject);
		void DeleteRenderObject(RenderObject* renderObject);

		void RegistRenderMesh(RenderObject& renderObject);
		void RegistRenderShadow(uint32 type, RenderObject& renderObject);

		void Excute();
		void ExcuteRenderPass(Graphics::RenderPass* renderPass);
		void Present();
		
		Graphics::RenderPass* GetRenderPass(uuid uuid);

	private:

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
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Bump_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Albedo_Bump_MRA_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Bump_Pass;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass;

		std::shared_ptr<Graphics::RenderPass> m_CascadedShadow_Pass;
		std::shared_ptr<Graphics::RenderPass> m_CascadedShadow_Skinned_Pass;

		std::shared_ptr<Graphics::RenderPass> m_PointShadow_Pass;
		std::shared_ptr<Graphics::RenderPass> m_PointShadow_Skinned_Pass;

		MaterialBuffer* m_Deferred_Light_Material;
		MeshBuffer* m_Screen_Mesh;
		std::shared_ptr<Graphics::RenderPass> m_Deferred_Light_Pass;


		std::shared_ptr<Graphics::RenderPass> m_Final_Pass;

		std::vector<std::shared_ptr<LightBuffer>> m_LightBuffers;

		std::shared_ptr<IBL> m_IBL;
		std::shared_ptr<Light> m_Light;
		std::shared_ptr<RenderQueue> m_RenderQueue;
		std::shared_ptr<Deferred> m_Deferred;
		std::shared_ptr<Sky> m_Sky;
		std::shared_ptr<PostProcess> m_SSR_Pass;

		std::shared_ptr<Voxel> m_Voxel_Pass;

		std::shared_ptr<DebugDeferred> m_Debug_Deferred;

		std::vector<RenderObject*> m_RenderObjectList;
	};

	extern "C"
	{
		GRAPHICSENGINE_DLL_DECLSPEC GraphicsEngine* CreateGraphicsEngine(GraphicsEngineDesc& desc);
		GRAPHICSENGINE_DLL_DECLSPEC void ReleaseGraphicsEngine(GraphicsEngine* graphicsEngine);
	}
}