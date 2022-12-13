#pragma once

#include "Common.h"

#include "GameEngine\Core\Resource\Resource.h"

namespace Sound
{
	class Sound;
}

namespace GameEngine
{
	namespace Core
	{
		class AudioClip
			: public Resource, public std::enable_shared_from_this<AudioClip>
		{
		public:
			AudioClip(const tstring& _typeName = TEXT("AudioClip"));
			virtual ~AudioClip();

		protected:

		private:
		};
	}
}