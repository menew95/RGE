#include "Importer_pch.h"
#include "Importer/FBX/FBXImporter.h"
#include "fbxsdk.h"

namespace Utility
{
	using namespace fbxsdk;

	inline Math::Vector4 Convert(fbxsdk::FbxVector4& v, bool lh = true)
	{
		if (lh == false)
		{
			return Math::Vector4
			(
				static_cast<float>(v.mData[0]),
				static_cast<float>(v.mData[1]),
				static_cast<float>(v.mData[2]),
				static_cast<float>(v.mData[3])
			);
		}

		// xyzw -> xzyw
		return Math::Vector4
		(
			static_cast<float>(v.mData[0]),
			static_cast<float>(v.mData[2]),
			static_cast<float>(v.mData[1]),
			static_cast<float>(v.mData[3])
		);
	}

	inline Math::Vector3 Convert(fbxsdk::FbxDouble3 v, bool lh = true)
	{
		if (lh == false)
		{
			return Math::Vector3
			(
				static_cast<float>(v.mData[0]),
				static_cast<float>(v.mData[1]),
				static_cast<float>(v.mData[2])
			);
		}

		// xyz -> xzy
		return Math::Vector3
		(
			static_cast<float>(v.mData[0]),
			static_cast<float>(v.mData[2]),
			static_cast<float>(v.mData[1])
		);
	}

	inline Math::Matrix Convert(fbxsdk::FbxMatrix& matrix, bool lh = true)
	{
		Math::Matrix _result;

		FbxVector4 r1 = matrix.GetRow(0);
		FbxVector4 r2 = matrix.GetRow(1);
		FbxVector4 r3 = matrix.GetRow(2);
		FbxVector4 r4 = matrix.GetRow(3);

		if (lh == false)
		{
			return Math::Matrix
			(
				Convert(r1, lh),
				Convert(r2, lh),
				Convert(r3, lh),
				Convert(r4, lh)
			);
		}

		return Math::Matrix
		(
			Convert(r1, lh),
			Convert(r3, lh),
			Convert(r2, lh),
			Convert(r4, lh)
		);
	}

	inline Math::Matrix Convert(fbxsdk::FbxAMatrix& aMatrix, bool lh = true)
	{
		fbxsdk::FbxMatrix matrix = aMatrix;

		return Convert(matrix, lh);
	}

	inline void Rotate(Math::Matrix& matrix)
	{
		const auto _pitch = -90.0f * Math::Deg2Rad;

		Math::Quaternion _q = Math::Quaternion::CreateFromYawPitchRoll(0, _pitch, 0);

		matrix *= Math::Matrix::CreateFromQuaternion(_q);
	}

	void ReconstructionName(tstring& name, const wchar_t* c)
	{
		if (name.find(c) != tstring::npos)
		{
			int idx = name.find(c) + 1;

			name.erase(0, idx);

			ReconstructionName(name, c);
		}
	}

	void RemoveExtension(tstring& name)
	{
		int idx = name.find(L".") + 1;

		size_t dot_pos = name.find_last_of(L".");
		if (dot_pos != std::wstring::npos)
		{
			name = name.substr(0, dot_pos);
		}
	}

	inline FbxAMatrix GetTransformMatrix(FbxNode* node)
	{
		const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
		return FbxAMatrix(translation, rotation, scaling);
	}

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

	void FBXImporter::LoadFile(const tstring& filePath, PrefabData& prefabData)
	{
		std::string _file = StringHelper::WStringToString(filePath);

		if (!m_FbxImporter->Initialize(_file.c_str(), -1, m_FbxManager->GetIOSettings()))
		{
			FbxString error = m_FbxImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

			auto _code = m_FbxImporter->GetStatus().GetCode();

			return;
		}


		if(!m_FbxImporter->Import(m_FbxScene.get()))
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

		prefabData._filePath = filePath;// StringHelper::StringToWString(m_FbxScene->GetName());
		prefabData._name = filePath;// StringHelper::StringToWString(m_FbxScene->GetName());

		ReconstructionName(prefabData._name, L"/");
		RemoveExtension(prefabData._name);

		FindAnimationData(prefabData);
		FbxNode* _rootNode = m_FbxScene->GetRootNode();

		TraversalNode(_rootNode, prefabData);
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

	void FBXImporter::FindAnimationData(PrefabData& prefabData)
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
			ReconstructionName(_newClip._clipName, L"|");

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

	void FBXImporter::LoadAnimationData(fbxsdk::FbxNode* node, PrefabData& prefabData)
	{
		const int animCount = static_cast<const int>(prefabData._animationClipDatas.size());

		for (int animIdx = 0; animIdx < animCount; animIdx++)
		{
			AnimationSnapData _snapData;

			_snapData._nodeName = StringHelper::StringToWString(node->GetName());

			FbxNode* _parentNode = node->GetParent();

			if (_parentNode != nullptr)
			{
				_snapData._parentNodeName = StringHelper::StringToWString(_parentNode->GetName());
			}

			FbxTime::EMode timeMode = m_FbxScene->GetGlobalSettings().GetTimeMode();

			std::string _animName = StringHelper::WStringToString(prefabData._animationClipDatas[animIdx]._clipName);

			FbxAnimStack* animStack = m_FbxScene->FindMember<FbxAnimStack>(_animName.c_str());
			m_FbxScene->SetCurrentAnimationStack(OUT animStack);

			for (FbxLongLong frame = 0; frame < prefabData._animationClipDatas[animIdx]._totalKeyFrame; frame++)
			{
				KeyFrame keyFrameInfo;

				FbxTime fbxTime = 0;

				fbxTime.SetFrame(frame, timeMode);

				// Local Transform = 부모 Bone의 Global Transform의 inverse Transform * 자신 Bone의 Global Transform;
				FbxAMatrix localTransform = node->EvaluateGlobalTransform(fbxTime);

				Math::Matrix localTM;

				if (FbxNode* parent = node->GetParent())
				{
					FbxNodeAttribute* ParentAttribute = parent->GetNodeAttribute();

					//if (ParentAttribute && ParentAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
					{
						FbxAMatrix GlobalParentTransform = parent->EvaluateGlobalTransform(fbxTime);

						localTransform = GlobalParentTransform.Inverse() * localTransform;

						localTM = Convert(localTransform);
					}
					/*else
					{
						FbxAMatrix GlobalParentTransform = parent->EvaluateGlobalTransform(fbxTime);

						localTransform = GlobalParentTransform.Inverse() * localTransform;

						localTM = Convert(localTransform);
					}*/
				}
				else
				{
					localTM = Convert(localTransform);
				}

				localTM.Decompose(keyFrameInfo._scale, keyFrameInfo._rot, keyFrameInfo._pos);

				keyFrameInfo._time = static_cast<float>(fbxTime.GetSecondDouble());

				_snapData._keyFrameList.push_back(keyFrameInfo);
			}

			prefabData._animationClipDatas[animIdx]._snapList.push_back(_snapData);
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

			// Transparency Data
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

			// Transparency Data
			_materialData._transparency = static_cast<float>(_lambertMaterial->TransparencyFactor.Get());
		}

		_materialData._albedoMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
		_materialData._normalMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
		_materialData._metalicRoughnessMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
		_materialData._emissiveMapTexture = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

		// 머터리얼 리스트에 추가
		prefabData._materialDataMap.insert(std::make_pair(_matName, _materialData));
	}

	void FBXImporter::LoadMesh(PrefabData& prefabData, fbxsdk::FbxMesh* meshNode, MeshData& meshData, int subMeshCnt)
	{
		meshData._meshName = StringHelper::StringToWString(meshNode->GetName());

		uint32 _vertexCount = meshNode->GetControlPointsCount();

		std::vector<Common::VertexAttribute> _tempMeshVertexList;
		_tempMeshVertexList.resize(_vertexCount);

		FbxVector4* _controlPoints = meshNode->GetControlPoints();

		for (uint32 i = 0; i < _vertexCount; i++)
		{
			_tempMeshVertexList[i]._position =
			{
				static_cast<float>(_controlPoints[i].mData[0]),
				static_cast<float>(_controlPoints[i].mData[2]),
				static_cast<float>(_controlPoints[i].mData[1])
			};
		}

		const int _deformerCount = meshNode->GetDeformerCount(FbxDeformer::eSkin);

		for (int i = 0; i < _deformerCount; i++)
		{
			meshData._isSkin = true;

			meshData._skinName = prefabData._name;

			FbxSkin* _fbxSkin = static_cast<FbxSkin*>(meshNode->GetDeformer(i, FbxDeformer::eSkin));

			if(_fbxSkin != nullptr)
			{
				FbxSkin::EType _type = _fbxSkin->GetSkinningType();

				if (_type == FbxSkin::eRigid || FbxSkin::eLinear)
				{
					const int _clusterCount = _fbxSkin->GetClusterCount();

					for (int j = 0; j < _clusterCount; j++)
					{
						FbxCluster* _cluster = _fbxSkin->GetCluster(j);

						if (_cluster->GetLink() == nullptr)
							continue;

						int _boneIdx = FindBoneIndex(_cluster->GetLink()->GetName(), prefabData);
						assert(_boneIdx >= 0);

						const int _indicesCount = _cluster->GetControlPointIndicesCount();

						for (int k = 0; k < _indicesCount; k++)
						{
							auto _weight = _cluster->GetControlPointWeights()[k];

							int _vtxIdx = _cluster->GetControlPointIndices()[k];

							for (int weightIdx = 0; weightIdx < 8; weightIdx++)
							{
								if (_tempMeshVertexList[_vtxIdx]._bone[weightIdx] == -1)
								{
									_tempMeshVertexList[_vtxIdx]._weight[weightIdx] = static_cast<float>(_weight);

									_tempMeshVertexList[_vtxIdx]._bone[weightIdx] = _boneIdx;

									break;
								}
							}
						}

						FbxAMatrix geometryTransform = GetTransformMatrix(meshNode->GetNode());
						FbxAMatrix transformMatrix;
						FbxAMatrix transformLinkMatrix;
						FbxAMatrix globalBindposeInverseMatrix;
						_cluster->GetTransformMatrix(transformMatrix);
						// The transformation of the mesh at binding time 
						_cluster->GetTransformLinkMatrix(transformLinkMatrix);
						// The transformation of the cluster(joint) at binding time from joint space to world space 
						globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;
						Math::Matrix _globalBindposeInverseMatrix = Convert(globalBindposeInverseMatrix, true);

						fbxsdk::FbxAMatrix matClusterTransformMatrix;
						fbxsdk::FbxAMatrix matClusterLinkTransformMatrix;

						_cluster->GetTransformMatrix(matClusterTransformMatrix);
						_cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);

						// Bone Matrix 설정..
						Math::Matrix _clusterMatrix = Convert(matClusterTransformMatrix, true);
						Math::Matrix _clusterlinkMatrix = Convert(matClusterLinkTransformMatrix, true);

						// BindPose 행렬을 구하자
						fbxsdk::FbxAMatrix _geometryTransform = GetTransformMatrix(meshNode->GetNode());
						Math::Matrix _geometryMatrix = Convert(_geometryTransform, true);

						// OffsetMatrix는 WorldBindPose의 역행렬
						Math::Matrix _offsetMatrix = _clusterMatrix * _clusterlinkMatrix.Invert();//* _geometryMatrix;

						prefabData._boneDatas[_boneIdx]._offsetTM = _offsetMatrix;
					}
				}
			}
		}

		const int polygonSize = meshNode->GetPolygonSize(0);

		assert(polygonSize == 3);		// 삼각형이 아니면 오류

		int arrIdx[3];
		int vertexCounter = 0;			// 정점의 개수

		const int triCount = meshNode->GetPolygonCount(); // 메쉬의 삼각형 개수를 가져온다

		std::map<std::tuple<unsigned, float, float, float, float, float>, unsigned> indexMap;

		/*if (!meshNode->CheckIfVertexNormalsCCW())
		{
			m_IsNegativeScaleFlag = false;
		}
		else
		{
			m_IsNegativeScaleFlag = true;
		}*/

		for (int i = 0; i < triCount; i++) // 삼각형의 개수
		{
			for (int j = 0; j < 3; j++)	   // 삼각형은 세 개의 정점으로 구성
			{
				int controlPointIndex = meshNode->GetPolygonVertex(i, j); // 제어점의 인덱스 추출

				arrIdx[j] = controlPointIndex;

				Math::Vector3 normal;

				normal = GetNormal(meshNode, controlPointIndex, vertexCounter);

				Math::Vector2 uv;

				uv = GetUV(meshNode, controlPointIndex, vertexCounter);

				const auto indexPair = std::make_tuple(controlPointIndex, uv.x, uv.y, normal.x, normal.y, normal.z);

				const auto iter = indexMap.find(indexPair);

				// map 에 없으면 insert 및 새로운 버텍스 제작
				if (iter == indexMap.end())
				{
					Common::VertexAttribute _vertex;
					_vertex._position = _tempMeshVertexList[controlPointIndex]._position;	// 포지션은 동일

					// 가중치 정보 동일
					for (int weightIdx = 0; weightIdx < 4; weightIdx++)
					{
						_vertex._weight[weightIdx] = _tempMeshVertexList[controlPointIndex]._weight[weightIdx];

						_vertex._bone[weightIdx] = _tempMeshVertexList[controlPointIndex]._bone[weightIdx];
					}

					_vertex._uv = uv;

					_vertex._normal = normal;

					meshData._vertexAttributes.push_back(_vertex);								// 새로운 버텍스 삽입

					controlPointIndex = static_cast<int>(meshData._vertexAttributes.size()) - 1;				// index 새로운 버텍스 껄로 바꾸기

					arrIdx[j] = controlPointIndex;

					indexMap.insert({ indexPair, controlPointIndex });
				}
				else // map 에 있던거라면
				{
					arrIdx[j] = iter->second;
				}

				vertexCounter++;
			}

			//if (meshNode->CheckIfVertexNormalsCCW())
			{
				meshData._indexAttributes[subMeshCnt].push_back(arrIdx[0]);
				meshData._indexAttributes[subMeshCnt].push_back(arrIdx[2]);
				meshData._indexAttributes[subMeshCnt].push_back(arrIdx[1]);
			}
			/*else
			{
				meshData._indexAttributes[subMeshCnt].push_back(arrIdx[0]);
				meshData._indexAttributes[subMeshCnt].push_back(arrIdx[2]);
				meshData._indexAttributes[subMeshCnt].push_back(arrIdx[1]);
			}*/
		}
	}

	Math::Vector3 FBXImporter::GetNormal(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter)
	{
		Math::Vector3 _normal;

		if (mesh->GetElementNormalCount() >= 1)
		{
			FbxGeometryElementNormal* normal = mesh->GetElementNormal();
			int normalIdx = 0;

			// 인덱스를 기준으로 노멀 값이 들어간다
			// 버텍스 스플릿이 필요하다.
			if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
					normalIdx = vertexCounter;
				else
					normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
			}
			// 정점을 기준으로 노멀 값이 들어간다.
			else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
					normalIdx = controlPointIndex;
				else
					normalIdx = normal->GetIndexArray().GetAt(controlPointIndex);
			}

			FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);

			if (m_IsNegativeScaleFlag)
			{
				_normal.x = -static_cast<float>(vec.mData[0]);
				_normal.y = -static_cast<float>(vec.mData[2]);
				_normal.z = -static_cast<float>(vec.mData[1]);
			}
			else
			{
				_normal.x = static_cast<float>(vec.mData[0]);
				_normal.y = static_cast<float>(vec.mData[2]);
				_normal.z = static_cast<float>(vec.mData[1]);
			}

			return _normal;
		}

		_normal = { -1.f, -1.f, -1.f };

		return _normal;
	}

	void FBXImporter::GetTangent(MeshData& meshInfo, int meshCnt)
	{
		for (int i = 0; i < meshInfo._indexAttributes[meshCnt].size(); i += 3) // 삼각형의 개수
		{
			int _i0 = meshInfo._indexAttributes[meshCnt][i];
			int _i1 = meshInfo._indexAttributes[meshCnt][i + (int)1];
			int _i2 = meshInfo._indexAttributes[meshCnt][i + (int)2];

			// e1 = p1 - p0, e2 = p2 - p0
			Math::Vector3 _e1 = meshInfo._vertexAttributes[_i1]._position - meshInfo._vertexAttributes[_i0]._position;
			Math::Vector3 _e2 = meshInfo._vertexAttributes[_i2]._position - meshInfo._vertexAttributes[_i0]._position;

			float _x1 = meshInfo._vertexAttributes[_i1]._uv.x - meshInfo._vertexAttributes[_i0]._uv.x; // u1 - u0
			float _y1 = meshInfo._vertexAttributes[_i1]._uv.y - meshInfo._vertexAttributes[_i0]._uv.y; // v1 - v0
			float _x2 = meshInfo._vertexAttributes[_i2]._uv.y - meshInfo._vertexAttributes[_i0]._uv.x; // u2 - u0
			float _y2 = meshInfo._vertexAttributes[_i2]._uv.y - meshInfo._vertexAttributes[_i0]._uv.y; // v2 - v0

			float _det = (_x1 * _y2) - (_x2 * _y1); // a * d - b * c
			if (_det == 0.f) { _det = 0.00001f; }
			float _r = 1.0f / _det;

			Math::Vector3 _tangent = (_e1 * _y2 - _e2 * _y1) * _r;

			meshInfo._vertexAttributes[_i0]._tangent += _tangent;
			meshInfo._vertexAttributes[_i1]._tangent += _tangent;
			meshInfo._vertexAttributes[_i2]._tangent += _tangent;
		}
	}

	Math::Vector2 FBXImporter::GetUV(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter)
	{
		Math::Vector2 _newUV;

		if (mesh->GetElementUVCount() >= 1)
		{
			int _uvIndex = 0;

			FbxGeometryElementUV* _fxbUV = mesh->GetLayer(0)->GetUVs();

			if (_fxbUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			{
				if (_fxbUV->GetReferenceMode() == FbxGeometryElement::eDirect)
					_uvIndex = vertexCounter;
				else
					_uvIndex = _fxbUV->GetIndexArray().GetAt(vertexCounter);
			}
			else if (_fxbUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				if (_fxbUV->GetReferenceMode() == FbxGeometryElement::eDirect)
					_uvIndex = controlPointIndex;
				else
					_uvIndex = _fxbUV->GetIndexArray().GetAt(controlPointIndex);
			}

			_newUV.x = static_cast<float>(_fxbUV->GetDirectArray().GetAt(_uvIndex).mData[0]);
			_newUV.y = 1.f - static_cast<float>(_fxbUV->GetDirectArray().GetAt(_uvIndex).mData[1]);

			if (_newUV.x < 0)
				_newUV.x += 1;

			return _newUV;
		}

		_newUV = { 0.f, 0.f };

		return _newUV;
	}

	void FBXImporter::FindBoneNode(fbxsdk::FbxNode* node, PrefabData& prefabData, int idx, int parentIdx)
	{
		fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();

		if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			BoneData _newBoneData;

			_newBoneData._boneName = StringHelper::StringToWString(node->GetName());

			_newBoneData._parentBoneIndex = parentIdx;

			_newBoneData._localTM = GetLocalMatrix(node);
			_newBoneData._worldTM = GetWorldMatrix(node);

			Rotate(_newBoneData._localTM);
			Rotate(_newBoneData._worldTM);

			prefabData._boneDatas.push_back(_newBoneData);

			LoadAnimationData(node, prefabData);
		}

		const int childCount = node->GetChildCount();

		for (int i = 0; i < childCount; i++)
		{
			if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
			{
				FindBoneNode(node->GetChild(i), prefabData, static_cast<int>(prefabData._boneDatas.size()), idx);
			}
			else
			{
				FindBoneNode(node->GetChild(i), prefabData, static_cast<int>(prefabData._boneDatas.size()), parentIdx);
			}
		}
	}

	void FBXImporter::TraversalNode(fbxsdk::FbxNode* node, PrefabData& prefabData)
	{
		fbxsdk::FbxNodeAttribute* _nodeAttribute = node->GetNodeAttribute();

#if defined(_DEBUG) || defined(DEBUG)
		auto _nodeName = node->GetName();

		if (_nodeAttribute && _nodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			BoneData _newBoneData;

			_newBoneData._boneName = StringHelper::StringToWString(node->GetName());

			_newBoneData._parentBoneName = StringHelper::StringToWString(node->GetParent()->GetName());

			_newBoneData._parentBoneIndex = FindBoneIndex(node->GetParent()->GetName(), prefabData);

			_newBoneData._localTM = GetLocalMatrix(node);
			_newBoneData._worldTM = GetWorldMatrix(node);

			/*Rotate(_newBoneData._localTM);
			Rotate(_newBoneData._worldTM);*/


			prefabData._boneDatas.push_back(_newBoneData);

			LoadAnimationData(node, prefabData);
		}
#endif
		if (_nodeAttribute != nullptr && _nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eNull)
		{
			GameObjectData _objectData;

			_objectData._gameObjectName = StringHelper::StringToWString(node->GetName());

			auto* _parentNode = node->GetParent();

			if (_parentNode != nullptr)
			{
				_objectData._hasParent = true;
				_objectData._parent = StringHelper::StringToWString(_parentNode->GetName());
			}

			_objectData._localTM = GetLocalMatrix(node);
			_objectData._worldTM = GetWorldMatrix(node);

			prefabData._gameObjectDatas.emplace_back(_objectData);
		}

		if (_nodeAttribute != nullptr && _nodeAttribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			GameObjectData _objectData;

			_objectData._gameObjectName = StringHelper::StringToWString(node->GetName());

			auto* _parentNode = node->GetParent();

			if (_parentNode != nullptr)
			{
				_objectData._hasParent = true;
				_objectData._parent = StringHelper::StringToWString(_parentNode->GetName());
			}

			_objectData._localTM = GetLocalMatrix(node);
			_objectData._worldTM = GetWorldMatrix(node);

			_objectData._hasMesh = true;

			float _det = _objectData._localTM.Determinant();

			// Negative scale 일 경우 negative scale flag 를 true로 설정
			if (_det < 0)
			{
				Math::Matrix _parentWorld = _objectData._worldTM * _objectData._localTM.Invert();

				m_IsNegativeScaleFlag = true;
				// Decompose 했다가 scale -주고 다시 합쳐야함..
				Math::Vector3 _scale;
				Math::Quaternion _rotQuat;
				Math::Vector3 _trans;
				_objectData._localTM.Decompose(_scale, _rotQuat, _trans);

				if (_scale.x < 0) _scale.x *= -1;
				if (_scale.y < 0) _scale.y *= -1;
				if (_scale.z < 0) _scale.z *= -1;

				// 다시 SRT 조립
				_objectData._localTM = Math::Matrix::CreateScale(_scale) * Math::Matrix::CreateFromQuaternion(_rotQuat) * Math::Matrix::CreateTranslation(_trans);
				
				_objectData._worldTM = _parentWorld * _objectData._localTM;
			}

			MeshData _newMeshData;

			_newMeshData._indexAttributes.resize(node->GetNodeAttributeCount());

			for (int _meshCnt = 0; _meshCnt < node->GetNodeAttributeCount(); _meshCnt++)
			{
				fbxsdk::FbxMesh* _meshNode = static_cast<fbxsdk::FbxMesh*>(node->GetNodeAttributeByIndex(_meshCnt));

				_objectData._mesh = StringHelper::StringToWString(_meshNode->GetName());

				LoadMesh(prefabData, _meshNode, _newMeshData, _meshCnt);

				fbxsdk::FbxLayerElementMaterial* _findMatIndex = _meshNode->GetElementMaterial(0);

				if (_findMatIndex != nullptr)
				{
					int index = _findMatIndex->GetIndexArray().GetAt(0);

					fbxsdk::FbxSurfaceMaterial* surfaceMaterial = _meshNode->GetNode()->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(index);

					LoadMaterial(surfaceMaterial, prefabData, _newMeshData);
				}
			}

			m_IsNegativeScaleFlag = false;

			prefabData._meshDataMap.insert(std::make_pair(_newMeshData._meshName, _newMeshData));

			prefabData._gameObjectDatas.emplace_back(_objectData);
		}

		const int _childCount = node->GetChildCount();

		for (int i = 0; i < _childCount; i++)
		{
			TraversalNode(node->GetChild(i), prefabData);
		}
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

	int32 FBXImporter::FindBoneIndex(std::string boneName, PrefabData& prefabData)
	{
		auto _boneName = StringHelper::StringToWString(boneName);

		auto _iter = std::find_if(std::begin(prefabData._boneDatas), std::end(prefabData._boneDatas), 
			[&_boneName](auto& v)
			{
				return v._boneName == _boneName;
			}
		);

		if (_iter != prefabData._boneDatas.end())
		{
			return static_cast<int32>(_iter - prefabData._boneDatas.begin());
		}
		else
		{
			return -1;
		}
	}

	Math::Matrix FBXImporter::GetLocalMatrix(fbxsdk::FbxNode* node, bool lh)
	{
		fbxsdk::FbxMatrix _localPos = m_FbxScene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

		Math::Matrix _localTM = Convert(_localPos, lh);

		auto _fbxtran = Convert(node->LclTranslation.Get(), lh);
		auto _fbxRot = Convert(node->LclRotation.Get(), lh);
		auto _fbxscale = Convert(node->LclScaling.Get(), lh);

		auto _rotQuat = Math::Quaternion::CreateFromYawPitchRoll(_fbxRot * Math::Deg2Rad);

		auto _scaleTM = Math::Matrix::CreateScale(_fbxscale);
		auto _rotTM = Math::Matrix::CreateFromQuaternion(_rotQuat);
		auto _transTM = Math::Matrix::CreateTranslation(_fbxtran);

		auto _localTM2 = Math::Matrix::CreateScale(_fbxscale) * Math::Matrix::CreateFromQuaternion(_rotQuat) * Math::Matrix::CreateTranslation(_fbxtran);

		auto _det = _localTM.Determinant();

		return _localTM;
	}

	Math::Matrix FBXImporter::GetWorldMatrix(fbxsdk::FbxNode* node, bool lh)
	{
		fbxsdk::FbxMatrix _globalPos = m_FbxScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

		Math::Matrix _worldTM = Convert(_globalPos, lh);

		Rotate(_worldTM);

		return _worldTM;
	}

}
