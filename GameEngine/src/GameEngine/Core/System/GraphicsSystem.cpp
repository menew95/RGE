#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/System/GraphicsSystem.h"

#include "Struct/VertexAttribute.h"

#include "GameEngine/Core/Resource/Mesh.h"
#include "GameEngine/Core/Resource/Material.h"

#include "GraphicsEngine/GraphicsEngine.h"

#include "GameEngine/Window.h"

HMODULE g_GraphicsEngineModule;

namespace GameEngine
{
	namespace Core
	{
		void* LoadProcedure(const char* procedureName)
		{
			auto procAddr = GetProcAddress(g_GraphicsEngineModule, procedureName);
			return reinterpret_cast<void*>(procAddr);
		}

		DEFINE_SINGLETON_CLASS(GraphicsSystem, {},
			{
				FreeGraphicsEngineDll();
			})

		
		void GraphicsSystem::Render()
		{
			m_GraphicsEngine->Excute();

			for (auto& _renderPass : m_RenderPassList)
			{

			}
		}

		void GraphicsSystem::Initialize()
		{
			LoadGraphicsEngineDll();
		}

		void GraphicsSystem::CreateMeshBuffer(std::shared_ptr<Mesh>& mesh)
		{
			uuid _uuid = mesh->GetName();

			mesh->m_MeshBuffer = m_GraphicsEngine->CreateMeshBuffer(_uuid, mesh->GetVertexAttributes(), mesh->GetIndexAttributes());
		}

		void GraphicsSystem::CreateMaterialBuffer(std::shared_ptr<Material>& material)
		{
			uuid _uuid = material->GetName();

			material->m_MaterialBuffer = m_GraphicsEngine->CreateMaterialBuffer(_uuid, TEXT("Deferred_Mesh_Layout"));
		}

		Graphics::CameraBuffer* GraphicsSystem::CreateCameraBuffer()
		{
			return m_GraphicsEngine->CreateCameraBuffer();
		}

		Graphics::LightBuffer* GraphicsSystem::CreateLightBuffer()
		{
			return m_GraphicsEngine->CreateLightBuffer();
		}

		Graphics::Texture* GraphicsSystem::LoadTexture(uuid _uuid)
		{
			Graphics::ImageDesc _desc;

			_desc._filePath = TEXT("Asset/Texture/") + _uuid;

			return m_GraphicsEngine->LoadTexture(_uuid, &_desc);
		}

		void GraphicsSystem::DeleteMeshBuffer(Graphics::MeshBuffer* meshBuffer)
		{
			// Todo :
		}

		void GraphicsSystem::DeleteMaterialBuffer(Graphics::MaterialBuffer* materialBuffer)
		{
			// Todo :
		}

		void GraphicsSystem::DeleteTextureBuffer(Graphics::Texture*)
		{
			// Todo :
		}

		void GraphicsSystem::DeleteLightBuffer(Graphics::LightBuffer*)
		{
			// Todo :
		}

		void GraphicsSystem::RegistRenderObject(const tstring& passName, Graphics::RenderObject& renderObject)
		{
			auto _find = std::find_if(std::begin(m_RenderPassList), std::end(m_RenderPassList),
				[&passName](auto& _pair)
				{
					return _pair.first == passName;
				}
			);

			assert(_find != m_RenderPassList.end());

			_find->second->RegistRenderObject(renderObject);
		}

		void GraphicsSystem::RegistRenderObject(uint32 passIdx, Graphics::RenderObject& renderObject)
		{
			m_RenderPassList[passIdx].second->RegistRenderObject(renderObject);
		}

		void GraphicsSystem::LoadGraphicsEngineDll()
		{
			std::string _moduleName = "GraphicsEngine";

			#ifdef _DEBUG
			#ifdef x64
						_moduleName += "_Debug_x64.dll";
			#else
						_moduleName += "_Debug_x86.dll";
			#endif // x64
			#else
			#ifdef x64
						_moduleName += "_Release_x64.dll";
			#else
						_moduleName += "_Release_x64.dll";
			#endif // x64
			#endif

			g_GraphicsEngineModule = LoadLibraryA(_moduleName.c_str());

			assert(g_GraphicsEngineModule != NULL);

			using CreateGraphicsEngine = Graphics::GraphicsEngine * (*)(Graphics::GraphicsEngineDesc&);

			auto createGraphicsEngine = (CreateGraphicsEngine)LoadProcedure("CreateGraphicsEngine");

			Graphics::GraphicsEngineDesc _desc;


			WindowInfo& _windowInfo = Window::GetInstance()->GetWindowInfo();

			_desc._handle = reinterpret_cast<void*>(_windowInfo._hWnd);

			_desc._isFullScreen = false;
			_desc._height = _windowInfo._height;
			_desc._width = _windowInfo._width;

			_desc._renderSystemDesc._systemType = Graphics::SystemType::DirectX11;

			auto _ptr = createGraphicsEngine(_desc);

			m_GraphicsEngine = std::make_unique<Graphics::GraphicsEngine>(*_ptr);

			m_RenderPassList.push_back(std::make_pair(TEXT("StaticMesh"), m_GraphicsEngine->GetMeshPass()));
			m_RenderPassList.push_back(std::make_pair(TEXT("StaticMeshBump"), m_GraphicsEngine->GetMeshBumpPass()));
			m_RenderPassList.push_back(std::make_pair(TEXT("StaticMeshBumpMRA"), m_GraphicsEngine->GetMeshBumpMRAPass()));
			m_RenderPassList.push_back(std::make_pair(TEXT("SkinnedMesh"), m_GraphicsEngine->GetSkinnedMeshPass()));
			m_RenderPassList.push_back(std::make_pair(TEXT("SkinnedMeshBump"), m_GraphicsEngine->GetSkinnedMeshBumpPass()));
			m_RenderPassList.push_back(std::make_pair(TEXT("SkinnedMeshBumpMRA"), m_GraphicsEngine->GetSkinnedMeshBumpMRAPass()));
		}

		void GraphicsSystem::FreeGraphicsEngineDll()
		{
			using ReleaseModule = void (*)(Graphics::GraphicsEngine*);

			auto releaseRenderSystem = (ReleaseModule)LoadProcedure("ReleaseGraphicsEngine");

			releaseRenderSystem(m_GraphicsEngine.get());

			m_GraphicsEngine.reset();
		}

	}
}