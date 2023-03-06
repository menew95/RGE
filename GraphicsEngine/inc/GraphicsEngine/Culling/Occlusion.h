#pragma once

namespace Graphics
{
	class CommandBuffer;
	class ResourceManager;

	class Occlusion
	{
	public:
		Occlusion(CommandBuffer* commandBuffer, ResourceManager* resourceManager);
		~Occlusion();

	private:

	};
}