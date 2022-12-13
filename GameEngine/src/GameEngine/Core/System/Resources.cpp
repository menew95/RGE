#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\System\Resources.h"

namespace GameEngine 
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(Resources, {}, {})

		void* Resources::Load(const tstring& filePath)
		{
			// Todo : 리소스 로드를 템플릿 함수로 만들지 정해야함
			return nullptr;
		}

	}
}