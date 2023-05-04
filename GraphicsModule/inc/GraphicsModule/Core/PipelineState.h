#pragma once

#include "Common.h"

#include "GraphicsModule/Core/RenderSystemObject.h"

#include "GraphicsModule/Core/PipelineStateFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC PipelineState : public RenderSystemObject
	{
	public:
		PipelineState() = default;
		virtual ~PipelineState() = default;

		//virtual void Bind(class StateManager* stateManager);
		virtual void SetShaderObjects(const ShaderProgram& shaderProgram) abstract;

	protected:

	};
}