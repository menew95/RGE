#pragma once

#include "Common.h"

#include "GameEngine\Core\Resource\Resource.h"

namespace Graphics
{
	class MaterialBuffer;
}

namespace GameEngine
{
	namespace Core
	{
		class GAME_ENGINE_API Material
			: public Resource, public std::enable_shared_from_this<Material>
		{
		public:
			Material(const tstring& _typeName = TEXT("Material"));
			virtual ~Material();

			inline std::shared_ptr<Graphics::MaterialBuffer>& GetResource() { return m_MaterialBuffer; }

		protected:
			virtual bool Release() override;

		private:
			std::shared_ptr<Graphics::MaterialBuffer> m_MaterialBuffer;
		};
	}
}