#pragma once

#include "GraphicsEngine/RenderObject.h"

/*
	∏≈Ω¨∏¶ ∫πºø»≠ Ω√≈¥
*/

namespace Graphics
{
	class CommandBuffer;
	class ResourceManager;

	class RenderPass;
	class Texture;

	//Structured Buffers
	struct VoxelType
	{
		uint32 color_mask;
		uint32 normal_mask;
	};

	class Voxel
	{
	public:
		Voxel(CommandBuffer* command, ResourceManager* resourceManager);
		~Voxel();

		void RegistRenderObject(RenderObject* renderObject);

		void Excute();

	private:
		void Initialize();

		void CreateVoxelResource();

		void CreateVoxelizePass();
		void CreateVoxelCopyPass();

		void UpdateResourcePerMaterial(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout);
		void UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject* renderObject, PipelineLayout* pipelineLayout);

		void UpdateBuffer(CommandBuffer* commandBuffer, Buffer* buffer, void* src, uint32 size);

		ResourceManager* m_ResourceManager = nullptr;
		CommandBuffer* m_CommandBuffer = nullptr;

		std::vector<RenderObject*> m_RenderObjectList;

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

#pragma endregion


		uint32 VOXEL_RESOLUTION = 128;
	};
}