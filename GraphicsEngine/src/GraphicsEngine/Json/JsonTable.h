#pragma once

#include "Math/Math.h"

namespace Graphics
{
	class ResourceManager;

	/**
		@class   TableLoader
		@brief   ���̺���� �ε��ϴ� �Լ���
		@details ~
		@author  KyungMin Oh
	**/
	class TableLoader
	{
	public:
		static bool LoadRenderTargetTable(ResourceManager* resourceManager, const Math::Vector2& screenSize);

		static bool LoadShaderTable(ResourceManager* resourceManager);

		static bool LoadPipelineStateTable(ResourceManager* resourceManager);

		static bool LoadRenderingPipelineTable(ResourceManager* resourceManager);

		static bool LoadTextureTable(ResourceManager* resourceManager);

	};
}