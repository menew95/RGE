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

			Graphics::Texture* GetAlbedoTexture() { return m_AlbedoMap; }
			Graphics::Texture* GetNormalTexture() { return m_NormalMap; }
			Graphics::Texture* GetMRATexture() { return m_MRAMap; }

		protected:
			virtual void Release() override;

		private:
			Graphics::Texture* m_AlbedoMap = nullptr;
			Graphics::Texture* m_NormalMap = nullptr;
			Graphics::Texture* m_MRAMap = nullptr;

			Graphics::MaterialBuffer* m_MaterialBuffer;
		};
	}
}