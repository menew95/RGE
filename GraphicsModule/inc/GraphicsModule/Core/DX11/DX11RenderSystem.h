#pragma once

#include "GraphicsModule\Core\RenderSystem.h"
#include "GraphicsModule\Core\DX11\Direct3D11.h"

namespace Graphics
{
	namespace DX11
	{
		class DX11RenderSystem : public RenderSystem
		{
		public: 
			DX11RenderSystem();
			~DX11RenderSystem() override;


		private:
			ComPtr<>
		};
	}
}