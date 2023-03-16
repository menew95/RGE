#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

/*
* Sky�� ǥ���ϴ� ���� �н�
* Skydom, SkyAtmosphere
*/

namespace Graphics
{
	class CameraBuffer;
	class RenderPass;
	class CommandBuffer;
	class ResourceManager;

	class Sky
	{
	public:
		Sky(CommandBuffer* command, ResourceManager* resourceManager);
		~Sky();

		void ExcutePass();

		void SetRenderTarget(RenderTarget* renderTarget);

	private:
		void Initialize();
		
		ResourceManager* m_ResourceManager;
		CommandBuffer* m_CommandBuffer;

		CameraBuffer* m_CameraBuffer;

		std::vector<RenderObject> m_RenderObjects;

		MaterialBuffer* m_SkyBox_Material;
		MeshBuffer* m_SkyBox_Mesh;
		std::shared_ptr<Graphics::RenderPass> m_SkyBox_Pass;
	};
}