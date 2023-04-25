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

		ResourceManager* m_ResourceManager = nullptr;
		CommandBuffer* m_CommandBuffer = nullptr;

		std::vector<RenderObject*> m_RenderObjectList;

		PipelineState* m_PipelineState = nullptr;
		PipelineLayout* m_PiprlineLayout = nullptr;
		
		Buffer* m_Voxel = nullptr;
		Texture* m_VoxelTexture = nullptr;
		Texture* m_VoxelBoundTexture = nullptr;

		RenderPass* m_VoxelizetionPass = nullptr;

#pragma region

		PipelineState* m_VoxelizePSO = nullptr;
		PipelineState* m_VoxelCopyCSO = nullptr;
		PipelineLayout* m_VoxelizeLayout = nullptr;

#pragma endregion


		uint32 VOXEL_RESOLUTION = 128;
	};
}