#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

/*
* PostProcess 관련 렌더패스를 다루는 클래스 일단은 SSR 하나
*/

namespace Graphics
{
	class RenderPass;
	class CommandBuffer;
	class ResourceManager;

	struct PostProcessSetting
	{
		// sslr
		Math::Vector2 _depthBufferSize = { 1280, 720 };
		float _zThickness = 0.1f;
		float _nearPlaneZ = 0.1f;

		float _stride = 1.0f;
		float _maxSteps = 300.f;
		float _maxDistance = 100.f;
		float _strideZCutoff = 1.f;

		float _numMips;
		float _fadeStart;
		float _fadeEnd;

		// tone map
		float _exposure = 1.0f;


		float _pad[4]; // 64bit
	};

	class PostProcess
	{
	public:
		PostProcess(CommandBuffer* command, ResourceManager* resourceManager);
		~PostProcess();

		void ExcutePass();

		void SetRenderTarget(RenderTarget* renderTarget);

		void SetPostProcessSetting(PostProcessSetting setting);

		Texture* GetBackBuffer() { return m_Buffer[m_CurrIdx]; }

	private:
		void Initialize();

		bool m_SSLR = false;
		bool m_ToneMap = false;

		PostProcessSetting m_PostProcessSetting;

		ResourceManager* m_ResourceManager;
		CommandBuffer* m_CommandBuffer;

		RenderObject m_RenderObjects;

		uint8 m_CurrIdx = 0;
		Texture* m_Buffer[2];
		RenderTarget* m_RenderTarget[2];

		std::shared_ptr<Graphics::RenderPass> m_PostProcess_Pass;
		std::shared_ptr<Graphics::RenderPass> m_PostProcess_Pass2;
	};
}