#include "Importer_pch.h"
#include "Importer/FBX/FBXImporter.h"
#include "fbxsdk.h"

namespace Utility
{
	using namespace fbxsdk;

	FBXImporter::FBXImporter() 
		: m_FbxManager(nullptr, [](FbxManager* ptr) {})
		, m_FbxImporter(nullptr, [](FbxImporter* ptr) {})
		, m_FbxScene(nullptr, [](FbxScene* ptr) {})
	{
		FBXInitialize();
	}

	FBXImporter::~FBXImporter()
	{

	}

	void FBXImporter::LoadFile(tstring filePath, PrefabData& prefabData)
	{
		std::string _file = StringHelper::WStringToString(filePath);

		if (m_FbxImporter->Initialize(_file.c_str(), -1, m_FbxManager->GetIOSettings()))
		{
			assert(false);
			return;
		}


		if(m_FbxImporter->Import(m_FbxScene.get()))
		{
			assert(false);
			return;
		}

		FbxSystemUnit lFbxFileSystemUnit = m_FbxScene->GetGlobalSettings().GetSystemUnit();
		FbxSystemUnit lFbxOriginSystemUnit = m_FbxScene->GetGlobalSettings().GetOriginalSystemUnit();

		double factor = lFbxOriginSystemUnit.GetScaleFactor();

		const FbxSystemUnit::ConversionOptions lConversionOptions =
		{
		  true,
		  true,
		  true,
		  true,
		  true,
		  true
		};

		lFbxFileSystemUnit.m.ConvertScene(m_FbxScene.get(), lConversionOptions);
		lFbxOriginSystemUnit.m.ConvertScene(m_FbxScene.get(), lConversionOptions);

		// GeometryConverter 객체 생성
		FbxGeometryConverter* _geometryConverter = new FbxGeometryConverter(m_FbxManager.get());

		// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다. 
		_geometryConverter->Triangulate(m_FbxScene.get(), true, true);

		// 여러 머터리얼이 하나의 메쉬에 할당된것을 하나의 메쉬가 하나의 머터리얼로 가지게함
		_geometryConverter->SplitMeshesPerMaterial(m_FbxScene.get(), true);

		prefabData._name = StringHelper::StringToWString(m_FbxScene->GetName());

		LoadAnimation(prefabData);

		FbxNode* _rootNode = m_FbxScene->GetRootNode();

		TraversalNode(_rootNode);
	}

	void FBXImporter::FBXInitialize()
	{
		m_FbxManager = std::move(std::unique_ptr<FbxManager, void(*)(FbxManager*)>(FbxManager::Create(),
			[](FbxManager* ptr) {ptr->Destroy(); }));

		FbxIOSettings* _pFbxIOSettings = FbxIOSettings::Create(m_FbxManager.get(), IOSROOT);
		m_FbxManager->SetIOSettings(_pFbxIOSettings);

		m_FbxImporter = std::move(std::unique_ptr<FbxImporter, void(*)(FbxImporter*)>(FbxImporter::Create(m_FbxManager.get(), ""),
			[](FbxImporter* ptr) {ptr->Destroy(); }));

		m_FbxScene = std::move(std::unique_ptr<FbxScene, void(*)(FbxScene*)>(FbxScene::Create(m_FbxManager.get(), ""),
			[](FbxScene* ptr) {ptr->Destroy(); }));

	}

	void FBXImporter::LoadAnimation(PrefabData& prefabData)
	{
		FbxArray<FbxString*> _animNames;
		m_FbxScene->FillAnimStackNameArray(OUT _animNames);

		const int animCount = _animNames.GetCount();

		for (int i = 0; i < animCount; i++)
		{
			FbxAnimStack* animStack = m_FbxScene->FindMember<FbxAnimStack>(_animNames[i]->Buffer());

			if (animStack == nullptr)
				continue;

			AnimationClipData _newClip;
			_newClip._clipName = StringHelper::StringToWString(animStack->GetName());

			// 시작시간, 종료시간, 초당 프레임에 대한 정보
			FbxTakeInfo* takeInfo = m_FbxScene->GetTakeInfo(animStack->GetName());
			double _startTime = takeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
			double _endTime = takeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
			double _frameRate = (float)FbxTime::GetFrameRate(m_FbxScene->GetGlobalSettings().GetTimeMode());

			_newClip._frameRate = static_cast<float>(_frameRate);

			if (_startTime < _endTime)
			{
				_newClip._totalKeyFrame = (int)((_endTime - _startTime) * (double)_frameRate);
				_newClip._endKeyFrame = (int)((_endTime - _startTime) * (double)_frameRate);
				_newClip._tickPerFrame = static_cast<uint32>((_endTime - _startTime) / _newClip._totalKeyFrame);
				_newClip._startKeyFrame = (int)(_startTime) *_newClip._totalKeyFrame;
			}

			prefabData._animationClipDatas.push_back(_newClip);
		}
	}

	void FBXImporter::LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, PrefabData& prefabData, MeshData& meshData)
	{
		tstring _matName = StringHelper::StringToWString(surfaceMaterial->GetName());

		meshData._materials.push_back(_matName);

		auto _find = prefabData._materialDataMap.find(_matName);

		if (_find != prefabData._materialDataMap.end())
		{
			return;
		}

		MaterialData _materialData;

		_materialData._matarialName = _matName;

		if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxSurfacePhong* _phongMaterial = reinterpret_cast<FbxSurfacePhong*>(surfaceMaterial);

			// Ambient Data
			_materialData._ambient.x = static_cast<float>(_phongMaterial->Ambient.Get()[0]) * 10.0f;
			_materialData._ambient.y = static_cast<float>(_phongMaterial->Ambient.Get()[1]) * 10.0f;
			_materialData._ambient.z = static_cast<float>(_phongMaterial->Ambient.Get()[2]) * 10.0f;
			_materialData._ambient.w = 1.0f;

			// Diffuse Data
			_materialData._diffuse.x = static_cast<float>(_phongMaterial->Diffuse.Get()[0]);
			_materialData._diffuse.y = static_cast<float>(_phongMaterial->Diffuse.Get()[1]);
			_materialData._diffuse.z = static_cast<float>(_phongMaterial->Diffuse.Get()[2]);
			_materialData._diffuse.w = 1.0f;

			// Specular Data
			_materialData._specular.x = static_cast<float>(_phongMaterial->Specular.Get()[0]);
			_materialData._specular.y = static_cast<float>(_phongMaterial->Specular.Get()[1]);
			_materialData._specular.z = static_cast<float>(_phongMaterial->Specular.Get()[2]);
			_materialData._specular.w = 1.0f;

			// Emissive Datam_FbxScene
			_materialData._emissive.x = static_cast<float>(_phongMaterial->Emissive.Get()[0]);
			_materialData._emissive.y = static_cast<float>(_phongMaterial->Emissive.Get()[1]);
			_materialData._emissive.z = static_cast<float>(_phongMaterial->Emissive.Get()[2]);
			_materialData._emissive.w = 1.0f;

			// Emissive Strength
			_materialData._emissionFactor = static_cast<float>(_phongMaterial->EmissiveFactor);

			// Transparecy Data
			_materialData._transparency = static_cast<float>(_phongMaterial->TransparencyFactor.Get());

			// Shininess Data
			_materialData._roughness = 1.0f - float(sqrt(fmax(static_cast<float>(_phongMaterial->Shininess.Get()), 0.0)) / 10.0);

			// 메탈이 맞을까?
			_materialData._metallic = static_cast<float>(_phongMaterial->ReflectionFactor.Get());

			// Reflectivity Data
			_materialData._reflectivity = static_cast<float>(_phongMaterial->ReflectionFactor.Get());
		}
		else if (surfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
		{
			FbxSurfaceLambert* _lambertMaterial = reinterpret_cast<FbxSurfaceLambert*>(surfaceMaterial);

			// Ambient Data
			_materialData._ambient.x = static_cast<float>(_lambertMaterial->Ambient.Get()[0]);
			_materialData._ambient.y = static_cast<float>(_lambertMaterial->Ambient.Get()[1]);
			_materialData._ambient.z = static_cast<float>(_lambertMaterial->Ambient.Get()[2]);
			_materialData._ambient.w = 1.0f;

			// Diffuse Data
			_materialData._diffuse.x = static_cast<float>(_lambertMaterial->Diffuse.Get()[0]);
			_materialData._diffuse.y = static_cast<float>(_lambertMaterial->Diffuse.Get()[1]);
			_materialData._diffuse.z = static_cast<float>(_lambertMaterial->Diffuse.Get()[2]);
			_materialData._diffuse.w = 1.0f;

			// Emissive Data
			_materialData._specular.x = static_cast<float>(_lambertMaterial->Emissive.Get()[0]);
			_materialData._specular.y = static_cast<float>(_lambertMaterial->Emissive.Get()[1]);
			_materialData._specular.z = static_cast<float>(_lambertMaterial->Emissive.Get()[2]);
			_materialData._specular.w = 1.0f;

			// Emissive Strength
			_materialData._emissionFactor = static_cast<float>(_lambertMaterial->EmissiveFactor);

			// Transparecy Data
			_materialData._transparency = static_cast<float>(_lambertMaterial->TransparencyFactor.Get());
		}

		_materialData._albedoMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
		_materialData._normalMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
		_materialData._metalicRoughnessMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
		_materialData._emissiveMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

		// 머터리얼 리스트에 추가
		prefabData._materialDataMap.insert(std::make_pair(_matName, _materialData));
	}

	void FBXImporter::TraversalNode(fbxsdk::FbxNode* node)
	{

	}

	tstring FBXImporter::GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty)
	{
		std::string name;

		FbxProperty textureProperty = surface->FindProperty(materialProperty);

		if (textureProperty.IsValid())
		{
			int count = textureProperty.GetSrcObjectCount();

			if (1 <= count)
			{
				FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
				if (texture)
					name = texture->GetRelativeFileName();
			}
		}

		tstring wstr;

		wstr.assign(name.begin(), name.end());

		tstring filename = std::filesystem::path(wstr).filename();

		return filename;
	}

}
