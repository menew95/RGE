#pragma once

#include "Common.h"
#include <PhysX\PxPhysicsAPI.h>

namespace Common
{
	class ErrorCallback;
}

namespace PhysicsModule
{
	class BaseErrorCallback : public physx::PxDefaultErrorCallback
	{
	public:
		BaseErrorCallback(Common::ErrorCallback* callBack)
		: m_pErrorCallback(callBack)
		{}
		virtual ~BaseErrorCallback() {}

		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
	
	private:

		Common::ErrorCallback* m_pErrorCallback;
	};
}