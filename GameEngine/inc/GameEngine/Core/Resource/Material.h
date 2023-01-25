#pragma once

#include "Common.h"

#include "GameEngine\Core\Resource\Resource.h"

namespace Graphics
{
	class MaterialBuffer;
	class Texture;
	class Resource;
}

namespace GameEngine
{
	namespace Core
	{
		class GAME_ENGINE_API Material
			: public Resource, public std::enable_shared_from_this<Material>
		{
			friend class GraphicsSystem;

		public:
			Material(const tstring& _typeName = TEXT("Material"));
			virtual ~Material();

			inline Graphics::MaterialBuffer* GetMaterialBuffer() { return m_MaterialBuffer; }

			void SetAlbedoTexture(Graphics::Texture* texture);
			void SetNormalTexture(Graphics::Texture* texture);
			void SetMRATexture(Graphics::Texture* texture);
		protected:
			virtual void Release() override;

		private:
			Graphics::MaterialBuffer* m_MaterialBuffer;
		};
	}
}