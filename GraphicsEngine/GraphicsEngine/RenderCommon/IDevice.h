#pragma once

#include "Prefix.h"
#include "../RenderCore/ID3D.h"

#include <memory>

struct _GUID;
typedef struct _GUID GUID;

namespace RenderCore
{
	class ViewportContext
	{

	};

	/// <summary>
	/// Device & Device Context
	/// </summary>
	class IDevice
	{
	public:

		virtual void* QueryInterface(const GUID& guid) abstract;

		virtual void Present() abstract;
		virtual std::shared_ptr<IDevice> CreateDevice() abstract;
	};
}