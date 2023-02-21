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

			void SetAlbedoColor(Math::Color color) { m_StandardResource._albedoColor = color; }
			void SetEmissiveColor(Math::Color color) { m_StandardResource._emissiveColor = color; }
			void SetRoughness(float value) { m_StandardResource._roughness = value; }
			void SetMetallic(float value) { m_StandardResource._metallic = value; }
			void SetTilling(Math::Vector2 tiling) { m_StandardResource._tiling = tiling; }

			Graphics::Texture* GetAlbedoTexture() { return m_AlbedoMap; }
			Graphics::Texture* GetNormalTexture() { return m_NormalMap; }
			Graphics::Texture* GetMRATexture() { return m_MRAMap; }

			Math::Color GetAlbedoColor() { return m_StandardResource._albedoColor; }
			Math::Color GetEmissiveColor() { return m_StandardResource._emissiveColor; }
			float GetRoughness() { return m_StandardResource._roughness; }
			float GetMetallic() { return m_StandardResource._metallic; }
			Math::Vector2 GetTilling() { return m_StandardResource._tiling; }

			void* GetStandardResource() { return &m_StandardResource; }

		protected:
			virtual void Release() override;

		private:
			struct Standard
			{
				Math::Color _albedoColor = Math::Color::White;
				Math::Color _emissiveColor = Math::Color::White;
				float _roughness = 0.0f;
				float _metallic = 0.0f;
				Math::Vector2 _tiling = Math::Vector2::Zero;
			};

			Standard m_StandardResource;

			Graphics::Texture* m_AlbedoMap = nullptr;
			Graphics::Texture* m_NormalMap = nullptr;
			Graphics::Texture* m_MRAMap = nullptr;

			Graphics::MaterialBuffer* m_MaterialBuffer = nullptr;

			RTTR_ENABLE(Resource);

			RTTR_REGISTRATION_FRIEND
		};
	}
}