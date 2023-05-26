#pragma once

#include "GraphicsEngine/RenderObject.h"

/*
	매쉬를 복셀화 시킴
*/

namespace Graphics
{
	class CommandBuffer;
	class ResourceManager;

	class RenderTarget;
	class RenderPass;
	class Texture;
	class MeshBuffer;

	//Structured Buffers
	struct VoxelType
	{
		uint32 color_mask;
		uint32 normal_mask;
	};

	DECLSPEC_ALIGN(16) struct VoxelInfoCB
	{
		Math::Vector3 grid_center;
		float data_size;        // voxel half-extent in world space units
		float data_size_rcp;    // 1.0 / voxel-half extent
		uint32 data_res;		// voxel grid resolution
		float data_res_rcp;     // 1.0 / voxel grid resolution
		uint32 num_cones;		
		float num_cones_rcp;
		float max_distance;
		float ray_step_size;
		uint32 mips;

		float _aoAlpha;			
		float _aoFalloff;
		float _inDirectFactor;
		uint32 _mode;

		float _temp;
	};

	class Voxel
	{
	public:
		Voxel(CommandBuffer* command, ResourceManager* resourceManager);
		~Voxel();

		void RegistRenderObject(RenderObject& renderObject);

		void Excute();

		void UpdateVoxelInfo(Vector3 camPos);

		inline void SetRenderTarget(RenderTarget* renderTarget)
		{
			m_RenderTarget = renderTarget;
		}

		void SetVoxelSetting(bool voxelgi, bool debug, bool line, bool boundce,
			uint32 frame, float voxelSize, uint32 coneNum, float rayStepDis,
			float maxDis, float aoAlpha, float aoFalloff, float inDirectFactor, uint32 mode, float temp = 1.f);

	private:
		void Culling();

		void Initialize();

		void CreateVoxelResource();

		void CreateVoxelizePass();
		void CreateVoxelCopyPass();

		void CreateVoxelDebugPass();

		void CreateVoxelGIPass();

		void UpdateResourcePerMaterial(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout);
		void UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout);

		void UpdateBuffer(CommandBuffer* commandBuffer, Buffer* buffer, void* src, uint32 size);

		void ExcuteVoxelize();

		void ExcuteCopy();

		void ExcuteDebug();

		void ExcuteVoxelGI();

		bool	m_VoxelGI = false;
		bool	m_VoxelDebug = false;
		bool	m_VoxelDebugLine = false;
		bool	m_VoxelSecondBounce = false;
		uint32	m_VoxelUpdateFrame = 1;

		ResourceManager* m_ResourceManager = nullptr;
		CommandBuffer* m_CommandBuffer = nullptr;

		RenderTarget* m_RenderTarget = nullptr;

		std::vector<RenderObject> m_RenderObjectList;


		PipelineState* m_PipelineState = nullptr;
		PipelineLayout* m_PiprlineLayout = nullptr;
		
		Buffer* m_Voxel = nullptr;
		Buffer* m_VoxelData = nullptr;
		Buffer* m_VoxelMipLevel = nullptr;
		Texture* m_VoxelTexture = nullptr;
		Texture* m_VoxelBoundTexture = nullptr;


		std::vector<Texture*> m_AnisotropicVoxelTextures;
		// Anisotropic Voxel Texture의 각 밉을 담고 있는 UAV Resources;
		std::vector<ResourceView*> m_MipResourceViews;

		RenderPass* m_VoxelizetionPass = nullptr;

#pragma region Render Pass

		// none texture
		PipelineState* m_VoxelizePSO = nullptr;
		PipelineLayout* m_VoxelizeLayout = nullptr;
		RenderPass* m_Voxelize_Pass = nullptr;
		
		// albedo map
		PipelineState* m_VoxelizePSO1 = nullptr;
		PipelineLayout* m_VoxelizeLayout1 = nullptr;
		RenderPass* m_Voxelize_Albedo_Pass = nullptr;

		// normal map
		PipelineState* m_VoxelizePSO2 = nullptr;
		PipelineLayout* m_VoxelizeLayout2 = nullptr;
		RenderPass* m_Voxelize_Albedo_Normal_Pass = nullptr;

		// mra map
		PipelineState* m_VoxelizePSO3 = nullptr;
		PipelineLayout* m_VoxelizeLayout3 = nullptr;
		RenderPass* m_Voxelize_Albedo_Nomal_MRA_Pass = nullptr;

		// voxel copy
		PipelineState* m_VoxelCopyCSO = nullptr;
		PipelineLayout* m_VoxelCopyLayout = nullptr;

		// aniso voxel
		PipelineState* m_AnisoVoxelCSO= nullptr;
		PipelineLayout* m_AnisoVoxelLayout = nullptr;

		// aniso voxel mip
		PipelineState* m_AnisoVoxelMipCSO = nullptr;
		PipelineLayout* m_AnisoVoxelMipLayout = nullptr;
		 
		// voxel debug
		PipelineState* m_VoxelDebugPSO = nullptr;
		PipelineState* m_VoxelDebugLinePSO = nullptr;
		PipelineLayout* m_VoxelDebugLayout = nullptr;
		
		// voxel gi
		PipelineState* m_VoxelGIPSO = nullptr;
		PipelineLayout* m_VoxelGILayout = nullptr;
		RenderTarget* m_VoxelGIRT = nullptr;
		MeshBuffer* m_ScreenMesh = nullptr;

#pragma endregion

		uint32 VOXEL_RESOLUTION = 128;

		VoxelInfoCB m_Voxel_Info;
	};
}