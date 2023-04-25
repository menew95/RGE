#pragma once

#include "GraphicsModule/Core/PipelineState.h"
#include "GraphicsModule/Core/Common/ComPtr.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Graphics
{
	namespace DX11
	{

		class DX11StateManager;

		class DX11ComputePS : public PipelineState
		{
		public:
			DX11ComputePS(ID3D11Device* device, const ComputePipelineDesc& desc);

			virtual void Bind(DX11StateManager& stateMngr);

		private:
			ComPtr<ID3D11ComputeShader> m_CS;
		};
	}
}