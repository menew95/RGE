#pragma once

#include "GameEngine\Core\Resource\Resource.h"
#include "Struct\VertexAttribute.h"

namespace Graphics
{
	class MeshBuffer;
}

namespace GameEngine
{
	namespace Core
	{
		struct BoneData
		{
			tstring _boneName;
			Math::Matrix _offsetTM;
		};

		struct SkinnedData
		{
			std::vector<BoneData> _boneDatas;
		};

		class GAME_ENGINE_API Mesh
			: public Resource, public std::enable_shared_from_this<Mesh>
		{
			friend class GraphicsSystem;
		public:
			Mesh(const tstring& typeName = TEXT("Mesh"));
			Mesh(std::vector<VertexAttribute>& vertices, std::vector<std::vector<uint32>>& indices, const tstring& typeName = TEXT("Mesh"));
			virtual ~Mesh();

			std::vector<VertexAttribute>& GetVertexAttributes() { return m_VertexAttributes; }
			std::vector<std::vector<uint32>>& GetIndexAttributes() { return m_IndexAttributers; }
			std::vector<uint32>& GetIndexAttributes(uint32 idx) { return m_IndexAttributers[idx]; }

			inline uint32 GetSubMeshCount() { return static_cast<uint32>(m_IndexAttributers.size()); }

			inline Graphics::MeshBuffer* GetMeshBuffer() { return m_MeshBuffer; }

			inline bool IsSkinned() { return m_Skinned; }

			inline tstring GetBoneName() { return m_BoneName; }
			inline void SetBoneName(tstring bone) { m_Skinned = true;  m_BoneName = bone; }
			inline void SetSkinned(std::shared_ptr<SkinnedData> skin) { m_SkinnedData = skin; }
			inline std::shared_ptr<SkinnedData>& GetSkinned() { return m_SkinnedData; }

		protected:
			virtual void Release() override;

		private:
			std::vector<VertexAttribute> m_VertexAttributes;
			std::vector<std::vector<uint32>> m_IndexAttributers;

			Graphics::MeshBuffer* m_MeshBuffer;

			bool m_Skinned = false;
			tstring m_BoneName;
			std::shared_ptr<SkinnedData> m_SkinnedData;

			RTTR_ENABLE(Resource);
			RTTR_REGISTRATION_FRIEND
		};
	}
}