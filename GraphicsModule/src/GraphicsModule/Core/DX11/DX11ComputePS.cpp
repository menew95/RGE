#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11ComputePS.h"
#include "GraphicsModule/Core/DX11/DX11Shader.h"
#include "GraphicsModule/Core/DX11/DX11StateManager.h"

namespace Graphics
{
	namespace DX11
	{

		DX11ComputePS::DX11ComputePS(ID3D11Device* device, const ComputePipelineDesc& desc)
		{
			if (desc._shaderProgram._computeShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(desc._shaderProgram._computeShader);
				m_CS = _castShader->GetNativeShader()._computeShader;
			}
			else
			{
				assert(false);
			}
		}

		void DX11ComputePS::Bind(DX11StateManager& stateMngr)
		{
			stateMngr.SetComputeShader(m_CS.Get());
		}

		void DX11ComputePS::SetShaderObjects(const ShaderProgram& shaderProgram)
		{
			if (shaderProgram._computeShader != nullptr)
			{
				auto _castShader = reinterpret_cast<DX11Shader*>(shaderProgram._computeShader);
				m_CS = _castShader->GetNativeShader()._computeShader;
			}
			else
			{
				assert(false);
			}
		}

	}
}