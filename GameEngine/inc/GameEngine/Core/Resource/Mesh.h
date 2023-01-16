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
		public:
			Mesh(const tstring& typeName = TEXT("Mesh"));
			virtual ~Mesh();

			std::vector<VertexAttribute>& GetVertexAttributes() { return m_VertexAttributes; }
			std::vector<uint32>& GetIndexAttributes() { return m_IndexAttributers; }

		protected:
			virtual bool Release() override;

		private:
			std::vector<VertexAttribute> m_VertexAttributes;
			std::vector<uint32> m_IndexAttributers;

			std::shared_ptr<Graphics::MeshBuffer> m_MeshBuffer;
		};
	}
}