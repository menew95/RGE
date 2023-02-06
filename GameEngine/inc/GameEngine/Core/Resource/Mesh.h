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

		protected:
			virtual void Release() override;

		private:
			std::vector<VertexAttribute> m_VertexAttributes;
			std::vector<std::vector<uint32>> m_IndexAttributers;

			Graphics::MeshBuffer* m_MeshBuffer;

			RTTR_ENABLE(Resource);
			RTTR_REGISTRATION_FRIEND
		};
	}
}