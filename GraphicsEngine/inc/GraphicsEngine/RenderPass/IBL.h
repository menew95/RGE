#pragma once

#include "GraphicsEngine/Resource/ConstantBuffer.h"

#define MAXMIPMAP 5

namespace Graphics
{
	class CommandBuffer;

	class RenderTarget;
	class RenderPass;
	class ResourceManager;

	class MaterialBuffer;
	class MeshBuffer;

	class IBL
	{
	public:
		IBL(CommandBuffer* commnad, ResourceManager* resourceManager);
		~IBL();

		void CreateIBLResource();

		void CreatePreFilteredMap();
		void CreateIrradianceMap();

	private:
		void CreatePreFilteredMip(uint32 mipLevel);

		void SetResources(ResourceManager* resourceManager);
		void CreateCubeMapMatrix();

		CommandBuffer* m_CommandBuffer;

		PerCubeMapMatrix m_PerCubeMapMatrix;
		PerPreFiltered m_PerPreFiltered;

		MaterialBuffer* m_Cube_Material;
		MeshBuffer* m_Cube_Mesh;

		RenderTarget* m_MipRenderTarget[MAXMIPMAP];

		std::shared_ptr<RenderPass> m_PreFiltered_Pass;
		std::shared_ptr<RenderPass> m_Irradiance_Pass;
	};
}