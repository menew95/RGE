#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\System\Resources.h"

namespace GameEngine 
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(Resources, {}, {})

		void* Resources::Load(const tstring& filePath)
		{
			// Todo : ���ҽ� �ε带 ���ø� �Լ��� ������ ���ؾ���
			return nullptr;
		}

	}
}