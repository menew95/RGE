#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\Metal\BaseErrorCallback.h"
#include "Common\ErrorCallback.h"


namespace PhysicsModule
{
	void BaseErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		__super::reportError(code, message, file, line);

		std::string _msg(message);

		_msg += file + '\n';
		_msg += line + '\n';

		switch (code)
		{
			case physx::PxErrorCode::eNO_ERROR:
			{
				m_pErrorCallback->Log(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eDEBUG_INFO:
			{
				m_pErrorCallback->Log(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eDEBUG_WARNING:
			{
				m_pErrorCallback->Waring(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eINVALID_PARAMETER:
			{
				m_pErrorCallback->Waring(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eINVALID_OPERATION:
			{
				m_pErrorCallback->Waring(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eOUT_OF_MEMORY:
			{
				m_pErrorCallback->Error(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eINTERNAL_ERROR:
			{
				m_pErrorCallback->Error(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eABORT:
			{
				m_pErrorCallback->Error(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::ePERF_WARNING:
			{
				m_pErrorCallback->Error(_msg.c_str());
				break;
			}
			case physx::PxErrorCode::eMASK_ALL:
			{
				m_pErrorCallback->Error(_msg.c_str());
				break;
			}
			default:
			{

				break;
			}
		}
	}
}