#pragma once

#include "GraphicsEngine/RenderObject.h"

/*
	∏≈Ω¨∏¶ ∫πºø»≠ Ω√≈¥
*/

namespace Graphics
{
	class CommandBuffer;
	class ResourceManager;

	class RenderTarget;
	class RenderPass;
	class Texture;

	//Structured Buffers
	struct VoxelType
	{
		uint32 color_mask;
		uint32 normal_mask;
	};

	DECLSPEC_ALIGN(16) struct VoxelInfoCB
	{
		Math::Vector3 grid_center;
		float   data_size;        // voxel half-extent in world space units
		float   data_size_rcp;    // 1.0 / voxel-half extent
		uint32    data_res;         // voxel grid resolution
		float   data_res_rcp;     // 1.0 / voxel grid resolution
		uint32    num_cones;
		float   num_cones_rcp;
		float   max_distance;
		float   ray_step_size;
		uint32    mips;
	};

	class Voxel
	{
	public:
		Voxel(CommandBuffer* command, ResourceManager* resourceManager);
		~Voxel();

		void RegistRenderObject(RenderObject& renderObject);

		void Excute();

		void UpdateVoxelInfo(Vector3 camPos, float voxelSize, float coneNum, float rayStepDis, float maxDis);

		inline void SetRenderTarget(RenderTarget* renderTarget)
		{
			m_RenderTarget = renderTarget;
		}

	private:
		void Initialize();

		void CreateVoxelResource();

		void CreateVoxelizePass();
		void CreateVoxelCopyPass();

		void CreateVoxelDebugPass();

		void UpdateResourcePerMaterial(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout);
		void UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout);

		void UpdateBuffer(CommandBuffer* commandBuffer, Buffer* buffer, void* src, uint32 size);

		void ExcuteVoxelize();

		void ExcuteCopy();

		void ExcuteDebug();

		ResourceManager* m_ResourceManager = nullptr;
		CommandBuffer* m_CommandBuffer = nullptr;

		RenderTarget* m_RenderTarget = nullptr;

		std::vector<RenderObject> m_RenderObjectList;

		PipelineState* m_PipelineState = nullptr;
		PipelineLayout* m_PiprlineLayout = nullptr;
		
		Buffer* m_Voxel = nullptr;
		Buffer* m_VoxelData = nullptr;
		Texture* m_VoxelTexture = nullptr;
		Texture* m_VoxelBoundTexture = nullptr;

		RenderPass* m_VoxelizetionPass = nullptr;

#pragma region

		PipelineState* m_VoxelizePSO = nullptr;
		PipelineLayout* m_VoxelizeLayout = nullptr;

		PipelineState* m_VoxelCopyCSO = nullptr;
		PipelineLayout* m_VoxelCopyLayout = nullptr;

		PipelineState* m_VoxelDebugPSO = nullptr;
		PipelineLayout* m_VoxelDebugLayout = nullptr;
#pragma endregion

		uint32 VOXEL_RESOLUTION = 128;

		VoxelInfoCB m_Voxel_Info;
	};
}