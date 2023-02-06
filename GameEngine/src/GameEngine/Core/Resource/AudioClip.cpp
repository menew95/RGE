#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Resource\AudioClip.h"

namespace GameEngine
{
	namespace Core
	{

		AudioClip::AudioClip(const tstring& _typeName /*= TEXT("AudioClip")*/)
			: Resource(_typeName)
		{

		}

		AudioClip::~AudioClip()
		{

		}

	}
}