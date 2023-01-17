#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/Json/JsonTable.h"
#include "GraphicsEngine/Json/JsonMacros.h"

#include "JsonReader/JsonReader.h"

#include "GraphicsEngine/RenderPass/RenderPass.h"
#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsModule/Core/Texture.h"
#include "GraphicsModule/Core/RenderTarget.h"

namespace Graphics
{

	bool TableLoader::LoadRenderTargetTable(ResourceManager* resourceManager, const Math::Vector2& screenSize)
	{
		auto* _jsonReader = Utility::JsonReader::GetInstance();

		auto _renderTargetTable = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/RenderTargetTable.json"));


		const TCHAR* UUID = _T("UUID");
		const TCHAR* sample = _T("Sample");
		const TCHAR* arrayLayer = _T("ArrayLayer");
		const TCHAR* mipLevels = _T("MipLevel");
		const TCHAR* extent = _T("Extent");
		const TCHAR* attachmentsCount = _T("AttachmentsCount");

		const TCHAR* renderTargetType = _T("RenderTargetType");
		const TCHAR* attachments = _T("Attachments");
		const TCHAR* resourceID = _T("ResourceID");

		auto _rtArray = _renderTargetTable->GetArray();
		uint32 _attachCount;

		for (uint32 _tableIdx = 0; _tableIdx < _rtArray.Size(); _tableIdx += _attachCount)
		{
			RenderTargetDesc _renderTargetDesc;
			uuid _uuid;

			if (_rtArray[_tableIdx].HasMember(UUID))
			{
				_uuid = _rtArray[_tableIdx][UUID].GetString();
			}

			if (_rtArray[_tableIdx].HasMember(extent))
			{
				uint32 _width = _rtArray[_tableIdx][extent][0].GetInt();
				uint32 _height = _rtArray[_tableIdx][extent][1].GetInt();
				_renderTargetDesc._extend = { _width, _height };
			}

			if (_rtArray[_tableIdx].HasMember(sample))
			{
				_renderTargetDesc._sample = _rtArray[_tableIdx][sample].GetInt();
			}

			if (!_rtArray[_tableIdx].HasMember(attachmentsCount)) assert(false);

			_attachCount = _rtArray[_tableIdx][attachmentsCount].GetInt();

			for (uint32 _attachIdx = 0; _attachIdx < _attachCount; _attachIdx++)
			{
				uint32 _idx = _tableIdx + _attachIdx;

				AttachmentDesc _attachmentDesc;

				if (_rtArray[_attachIdx].HasMember(renderTargetType))
				{
					auto string = _rtArray[_attachIdx][renderTargetType].GetString();

					if (!StringToEnum(_rtArray[_attachIdx][renderTargetType].GetString(), _attachmentDesc._renderTargetType))
					{
						assert("Error");
					}
				}

				if (_rtArray[_attachIdx].HasMember(resourceID))
				{
					auto _findID = _rtArray[_attachIdx][resourceID].GetString();

					_attachmentDesc._resource = resourceManager->GetTexture(_findID);
					assert(_attachmentDesc._resource != nullptr);
				}

				if (_rtArray[_attachIdx].HasMember(mipLevels))
				{
					_attachmentDesc._mipLevel = _rtArray[_attachIdx][mipLevels].GetInt();
				}

				if (_rtArray[_attachIdx].HasMember(arrayLayer))
				{
					_attachmentDesc._arrayLayer = _rtArray[_attachIdx][arrayLayer].GetInt();
				}

				_renderTargetDesc._attachments.push_back(_attachmentDesc);
			}

			resourceManager->CreateRenderTarget(_uuid, _renderTargetDesc);
		}

		/*for (auto& renderTarget : _renderTargetTable->GetArray())
		{
			RenderTargetDesc _renderTargetDesc;
			uuid _uuid;

			if (renderTarget.HasMember(UUID))
			{
				_uuid = renderTarget[UUID].GetString();
			}

			if (renderTarget.HasMember(extent))
			{
				if (renderTarget[extent].Size() > 2)
				{
					assert("Error");
				}

				for (uint32 i = 0; i < renderTarget[extent].Size(); i++)
				{
					if (i == 0)
						_renderTargetDesc._extend._width = renderTarget[extent][i].GetInt();
					if (i == 1)
						_renderTargetDesc._extend._height = renderTarget[extent][i].GetInt();
				}
			}

			if (renderTarget.HasMember(sample))
			{
				_renderTargetDesc._sample = renderTarget[sample].GetInt();
			}

			if (renderTarget.HasMember(attachments))
			{

				for (uint32 i = 0; i < renderTarget[attachments].Size(); i++)
				{
					AttachmentDesc _attachmentDesc;

					if (renderTarget[attachments][i].HasMember(renderTargetType))
					{
						if (!StringToEnum(renderTarget[attachments][i][renderTargetType].GetString(), _attachmentDesc._renderTargetType))
						{
							assert("Error");
						}
					}

					if (renderTarget[attachments][i].HasMember(resourceID))
					{
						auto _findID = renderTarget[attachments][i][resourceID].GetString();

						_attachmentDesc._resource = resourceManager->GetTexture(_findID);
						assert(_attachmentDesc._resource != nullptr);
					}

					if (renderTarget[attachments][i].HasMember(mipLevels))
					{
						_attachmentDesc._mipLevel = renderTarget[attachments][i][mipLevels].GetInt();
					}

					if (renderTarget[attachments][i].HasMember(arrayLayer))
					{
						_attachmentDesc._arrayLayer = renderTarget[attachments][i][arrayLayer].GetInt();
					}

					_renderTargetDesc._attachments.push_back(_attachmentDesc);
				}
			}
			else
			{
				assert("Error");
			}

			resourceManager->CreateRenderTarget(_uuid, _renderTargetDesc);
		}*/

		_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/RenderTargetTable.json"));

		return true;
	}

	bool TableLoader::LoadShaderTable(ResourceManager* resourceManager)
	{
		auto* _jsonReader = Utility::JsonReader::GetInstance();

		auto _shaderTables = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/ShaderTable.json"));

		const TCHAR* UUID = TEXT("UUID");
		const TCHAR* shaderType = TEXT("ShaderType");
		const TCHAR* shaderSourceType = TEXT("ShaderSourceType");
		const TCHAR* filePath = TEXT("Path");
		const TCHAR* sourceSize = TEXT("SourceSize");
		const TCHAR* entryPoint = TEXT("EntryPoint");
		const TCHAR* profile = TEXT("Profile");
		const TCHAR* defines = TEXT("Defines");
		const TCHAR* flags = TEXT("Flags");

		for (auto& _shaderTable : _shaderTables->GetArray())
		{
			ShaderDesc _shaderDesc;
			uuid _uuid;

			std::vector<ShaderMacro> _defines;
			std::vector<std::string> _definesName;
			std::vector<std::string> _definesDefinition;
			std::string _entryPoint;
			std::string _profile;

			if (_shaderTable.HasMember(UUID))
			{
				_uuid = _shaderTable[UUID].GetString();
			}

			if (_shaderTable.HasMember(shaderType))
			{
				if (!StringToEnum(_shaderTable[shaderType].GetString(), _shaderDesc._shaderType)) assert(false);
			}

			if (_shaderTable.HasMember(shaderSourceType))
			{
				if (!StringToEnum(_shaderTable[shaderSourceType].GetString(), _shaderDesc._sourceType)) assert(false);
			}

			if (_shaderTable.HasMember(filePath))
			{
				_shaderDesc._filePath = _shaderTable[filePath].GetString();
			}

			if (_shaderTable.HasMember(sourceSize))
			{
				_shaderDesc._sourceSize = _shaderTable[sourceSize].GetInt();
			}

			if (_shaderTable.HasMember(entryPoint))
			{
				_entryPoint = StringHelper::WStringToString(_shaderTable[entryPoint].GetString());
				_shaderDesc._entryPoint = _entryPoint.c_str();
			}

			if (_shaderTable.HasMember(profile))
			{
				_profile = StringHelper::WStringToString(_shaderTable[profile].GetString());
				_shaderDesc._profile = _profile.c_str();
			}

			if (_shaderTable.HasMember(defines))
			{
				auto _macroCnt = _shaderTable[defines].Size() / 2 + 1;
				_definesName.reserve(_macroCnt);
				_definesDefinition.reserve(_macroCnt);

				for (uint i = 0; i < _shaderTable[defines].Size(); i += 2)
				{
					ShaderMacro _macro;

					std::string _macroName = StringHelper::WStringToString(_shaderTable[defines][i].GetString());
					std::string _macroDefinition = StringHelper::WStringToString(_shaderTable[defines][i + 1].GetString());

					_definesName.push_back(_macroName);
					_definesDefinition.push_back(_macroDefinition);

					_macro.name = _definesName.back().c_str();
					_macro.definition = _definesDefinition.back().c_str();

					_defines.push_back(_macro);
				}


				_defines.push_back({ NULL, NULL });
				_shaderDesc.defines = _defines.data();
			}

			if (_shaderTable.HasMember(flags))
			{
				uint32 flags = 0;

				for (uint i = 0; i < _shaderTable[flags].Size(); i++)
				{

				}

				_shaderDesc._flags = flags;
			}

			resourceManager->CreateShader(_uuid, _shaderDesc);
		}

		_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/ShaderTable.json"));

		return true;
	}

	bool TableLoader::LoadPipelineStateTable(ResourceManager* resourceManager)
	{

		auto* _jsonReader = Utility::JsonReader::GetInstance();

		std::map<uuid, DepthDesc> _depthDescMap;
		std::map<uuid, StencilDesc> _stencilDescMap;
		std::map<uuid, RasterizerDesc> _rasterizerDescMap;
		std::map<uuid, BlendDesc> _blendDescMap;

		{
			auto _depthStencilTables = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/DepthStencilStateTable.json"));

			const TCHAR* UUID = TEXT("UUID");

			const TCHAR* depthEnabled = TEXT("DepthEnable");
			const TCHAR* compareOp = TEXT("CompareOp");
			const TCHAR* writeEnabled = TEXT("WriteEnabled");

			const TCHAR* stencilEnable = TEXT("StencilEnable");
			const TCHAR* readMask = TEXT("StencilReadMask");
			const TCHAR* writeMask = TEXT("StencilWriteMask");

			const TCHAR* front_StencilFailOp = TEXT("Front/StencilFailOp");
			const TCHAR* front_DepthFailOp = TEXT("Front/DepthFailOp");
			const TCHAR* front_DepthPassOp = TEXT("Front/DepthPassOp");
			const TCHAR* front_CompareOp = TEXT("Front/CompareOp");

			const TCHAR* back_StencilFailOp = TEXT("Back/StencilFailOp");
			const TCHAR* back_DepthFailOp = TEXT("Back/DepthFailOp");
			const TCHAR* back_DepthPassOp = TEXT("Back/DepthPassOp");
			const TCHAR* back_CompareOp = TEXT("Back/CompareOp");

			for (auto& _depthStencilTable : _depthStencilTables->GetArray())
			{
				DepthDesc _depthDesc;
				StencilDesc _stencilDesc;

				uuid _uuid;

				if (_depthStencilTable.HasMember(UUID))
				{
					_uuid = _depthStencilTable[UUID].GetString();
				}

				// depth

				if (_depthStencilTable.HasMember(depthEnabled))
				{
					_depthDesc._depthEnabled = _depthStencilTable[depthEnabled].GetBool();
				}

				if (_depthStencilTable.HasMember(compareOp))
				{
					if (!StringToEnum(_depthStencilTable[compareOp].GetString(), _depthDesc.compareOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(writeEnabled))
				{
					_depthDesc._writeEnabled = _depthStencilTable[writeEnabled].GetBool();
				}


				// stencil

				if (_depthStencilTable.HasMember(stencilEnable))
				{
					_stencilDesc._stencilEnable = _depthStencilTable[stencilEnable].GetBool();
				}

				if (_depthStencilTable.HasMember(readMask))
				{
					_stencilDesc._readMask = _depthStencilTable[readMask].GetInt();
				}

				if (_depthStencilTable.HasMember(writeMask))
				{
					_stencilDesc._writeMask = _depthStencilTable[writeMask].GetInt();
				}

				if (_depthStencilTable.HasMember(front_StencilFailOp))
				{
					if (!StringToEnum(_depthStencilTable[front_StencilFailOp].GetString(), _stencilDesc._front._stencilFailOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(front_DepthFailOp))
				{
					if (!StringToEnum(_depthStencilTable[front_DepthFailOp].GetString(), _stencilDesc._front._depthFailOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(front_DepthPassOp))
				{
					if (!StringToEnum(_depthStencilTable[front_DepthPassOp].GetString(), _stencilDesc._front._depthPassOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(front_CompareOp))
				{
					if (!StringToEnum(_depthStencilTable[front_CompareOp].GetString(), _stencilDesc._front._compareOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(back_StencilFailOp))
				{
					if (!StringToEnum(_depthStencilTable[back_StencilFailOp].GetString(), _stencilDesc._back._stencilFailOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(back_DepthFailOp))
				{
					if (!StringToEnum(_depthStencilTable[back_DepthFailOp].GetString(), _stencilDesc._back._depthFailOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(back_DepthPassOp))
				{
					if (!StringToEnum(_depthStencilTable[back_DepthPassOp].GetString(), _stencilDesc._back._depthPassOp))
					{
						assert(false);
					}
				}

				if (_depthStencilTable.HasMember(back_CompareOp))
				{
					if (!StringToEnum(_depthStencilTable[back_CompareOp].GetString(), _stencilDesc._back._compareOp))
					{
						assert(false);
					}
				}

				_depthDescMap.insert(std::make_pair(_uuid, _depthDesc));
				_stencilDescMap.insert(std::make_pair(_uuid, _stencilDesc));
			}

			_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/DepthStencilStateTable.json"));
		}

		{
			auto _rasterizerTables = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/RasterizerStateTable.json"));

			const TCHAR* UUID = TEXT("UUID");
			const TCHAR* fillMode = TEXT("FillMode");
			const TCHAR* cullMode = TEXT("CullMode");

			const TCHAR* constantFactor = TEXT("ConstantFactor");
			const TCHAR* slopeFactor = TEXT("SlopeFactor");
			const TCHAR* clamp = TEXT("Clamp");

			const TCHAR* frontCCW = TEXT("FrontCCW");

			const TCHAR* depthClampEnabled = TEXT("DepthClampEnabled");
			const TCHAR* scissorTestEnabled = TEXT("ScissorTestEnabled");
			const TCHAR* multiSampleEnabled = TEXT("MultiSampleEnabled");
			const TCHAR* antiAliasedLineEnabled = TEXT("AntiAliasedLineEnabled");
			const TCHAR* conservativeRasterization = TEXT("ConservativeRasterization");

			for (auto& _rasterizerTable : _rasterizerTables->GetArray())
			{
				RasterizerDesc _rasterizerDesc;
				uuid _uuid;

				if (_rasterizerTable.HasMember(UUID))
				{
					_uuid = _rasterizerTable[UUID].GetString();
				}

				if (_rasterizerTable.HasMember(fillMode))
				{
					if (!StringToEnum(_rasterizerTable[fillMode].GetString(), _rasterizerDesc._fillMode))
					{
						assert(false);
					}
				}

				if (_rasterizerTable.HasMember(cullMode))
				{
					if (!StringToEnum(_rasterizerTable[cullMode].GetString(), _rasterizerDesc._cullMode))
					{
						assert(false);
					}
				}

				if (_rasterizerTable.HasMember(constantFactor))
				{
					_rasterizerDesc._depthBias.constantFactor = _rasterizerTable[constantFactor].GetFloat();
				}

				if (_rasterizerTable.HasMember(slopeFactor))
				{
					_rasterizerDesc._depthBias.slopeFactor = _rasterizerTable[slopeFactor].GetFloat();
				}

				if (_rasterizerTable.HasMember(clamp))
				{
					_rasterizerDesc._depthBias.clamp = _rasterizerTable[clamp].GetFloat();
				}

				if (_rasterizerTable.HasMember(frontCCW))
				{
					_rasterizerDesc._frontCCW = _rasterizerTable[frontCCW].GetBool();
				}

				if (_rasterizerTable.HasMember(scissorTestEnabled))
				{
					_rasterizerDesc._scissorTestEnabled = _rasterizerTable[scissorTestEnabled].GetBool();
				}

				if (_rasterizerTable.HasMember(multiSampleEnabled))
				{
					_rasterizerDesc._multiSampleEnabled = _rasterizerTable[multiSampleEnabled].GetBool();
				}

				if (_rasterizerTable.HasMember(antiAliasedLineEnabled))
				{
					_rasterizerDesc._antiAliasedLineEnabled = _rasterizerTable[antiAliasedLineEnabled].GetBool();
				}

				if (_rasterizerTable.HasMember(conservativeRasterization))
				{
					_rasterizerDesc._conservativeRasterization = _rasterizerTable[conservativeRasterization].GetBool();
				}

				_rasterizerDescMap.insert(std::make_pair(_uuid, _rasterizerDesc));
			}

			_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/RasterizerStateTable.json"));
		}

		{
			auto _blendTables = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/BlendStateTable.json"));

			const TCHAR* UUID = TEXT("UUID");
			const TCHAR* alphaToCoverageEnable = TEXT("AlphaToCoverageEnable");
			const TCHAR* independentBlendEnable = TEXT("IndependentBlendEnable");

			//const TCHAR* renderTarget = TEXT("RenderTarget");
			const TCHAR* blendEnable = TEXT("BlendEnable");
			const TCHAR* srcBlend = TEXT("SrcBlend");
			const TCHAR* destBlend = TEXT("DestBlend");
			const TCHAR* blendOp = TEXT("BlendOp");
			const TCHAR* srcBlendAlpha = TEXT("SrcBlendAlpha");
			const TCHAR* destBlendAlpha = TEXT("DestBlendAlpha");
			const TCHAR* blendOpAlpha = TEXT("BlendOpAlpha");
			const TCHAR* renderTargetWriteMask = TEXT("RenderTargetWriteMask");

			const TCHAR* blendFactor = TEXT("BlendFactor");
			const TCHAR* sampleMask = TEXT("SampleMask");

			const TCHAR* blendFactorDynamic = TEXT("BlendFactorDynamic");

			for (auto& _blendTable : _blendTables->GetArray())
			{
				BlendDesc _blendDesc;
				uuid _uuid;

				if (_blendTable.HasMember(UUID))
				{
					_uuid = _blendTable[UUID].GetString();
				}

				if (_blendTable.HasMember(alphaToCoverageEnable))
				{
					_blendDesc._alphaToCoverageEnable = _blendTable[alphaToCoverageEnable].GetBool();
				}

				if (_blendTable.HasMember(independentBlendEnable))
				{
					_blendDesc._independentBlendEnable = _blendTable[independentBlendEnable].GetBool();
				}

				if (_blendTable.HasMember(blendEnable))
				{
					_blendDesc._renderTarget[0]._blendEnable = _blendTable[blendEnable].GetBool();
				}

				if (_blendTable.HasMember(srcBlend))
				{
					if (!StringToEnum(_blendTable[srcBlend].GetString(), _blendDesc._renderTarget[0]._srcBlend))
					{
						assert(false);
					}
				}

				if (_blendTable.HasMember(destBlend))
				{
					if (!StringToEnum(_blendTable[destBlend].GetString(), _blendDesc._renderTarget[0]._destBlend))
					{
						assert(false);
					}
				}

				if (_blendTable.HasMember(blendOp))
				{
					if (!StringToEnum(_blendTable[blendOp].GetString(), _blendDesc._renderTarget[0]._blendOp))
					{
						assert(false);
					}
				}

				if (_blendTable.HasMember(srcBlendAlpha))
				{
					if (!StringToEnum(_blendTable[srcBlendAlpha].GetString(), _blendDesc._renderTarget[0]._srcBlendAlpha))
					{
						assert(false);
					}
				}

				if (_blendTable.HasMember(destBlendAlpha))
				{
					if (!StringToEnum(_blendTable[destBlendAlpha].GetString(), _blendDesc._renderTarget[0]._destBlendAlpha))
					{
						assert(false);
					}
				}

				if (_blendTable.HasMember(blendOpAlpha))
				{
					if (!StringToEnum(_blendTable[blendOpAlpha].GetString(), _blendDesc._renderTarget[0]._blendOpAlpha))
					{
						assert(false);
					}
				}

				if (_blendTable.HasMember(renderTargetWriteMask))
				{
					_blendDesc._renderTarget[0]._renderTargetWriteMask = _blendTable[renderTargetWriteMask].GetInt();
				}

				if (_blendTable.HasMember(blendFactor))
				{
					for (uint32 i = 0; i < _blendTable[blendFactor].Size(); i++)
					{
						if(i == 0) _blendDesc._blendFactor.x = _blendTable[blendFactor][i].GetFloat();
						if(i == 1) _blendDesc._blendFactor.x = _blendTable[blendFactor][i].GetFloat();
						if(i == 2) _blendDesc._blendFactor.x = _blendTable[blendFactor][i].GetFloat();
						if(i == 3) _blendDesc._blendFactor.x = _blendTable[blendFactor][i].GetFloat();
					}
				}

				if (_blendTable.HasMember(sampleMask))
				{
					_blendDesc._sampleMask = _blendTable[sampleMask].GetInt();
				}

				if (_blendTable.HasMember(blendFactorDynamic))
				{
					_blendDesc._blendFactorDynamic = _blendTable[blendFactorDynamic].GetBool();
				}

				_blendDescMap.insert(std::make_pair(_uuid, _blendDesc));
			}

			_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/BlendStateTable.json"));
		}

		{
			auto _pipelineStateTables = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/PipelineStateTable.json"));

			const TCHAR* UUID = TEXT("UUID");
			const TCHAR* pipelineLayoutUUID = TEXT("PipelineLayout");
			const TCHAR* shaderProgram = TEXT("ShaderProgram");

			const TCHAR* primitiveTopology = TEXT("PrimitiveTopology");

			const TCHAR* viewport = TEXT("Viewport");
			const TCHAR* scissors = TEXT("Scissors");

			const TCHAR* depthDesc = TEXT("DepthDesc");
			const TCHAR* stencilDesc = TEXT("StencilDesc");
			const TCHAR* rasterizerDesc = TEXT("RasterizerDesc");
			const TCHAR* blendDesc = TEXT("BlendDesc");

			for (auto& _pipelineStateTable : _pipelineStateTables->GetArray())
			{
				GraphicsPipelineDesc _pipelineStateDesc;
				uuid _uuid;

				if (_pipelineStateTable.HasMember(UUID))
				{
					_uuid = _pipelineStateTable[UUID].GetString();
				}

				if (_pipelineStateTable.HasMember(pipelineLayoutUUID))
				{
					uuid _layoutUUID = _pipelineStateTable[pipelineLayoutUUID].GetString();

					_pipelineStateDesc._pipelineLayout = resourceManager->GetPipelineLayout(_layoutUUID);
				}

				if (_pipelineStateTable.HasMember(shaderProgram))
				{
					for (uint32 i = 0; i < _pipelineStateTable[shaderProgram].Size(); i++)
					{
						auto _shaderID = _pipelineStateTable[shaderProgram][i].GetString();

						if (_shaderID != TEXT("NULL"))
						{
							if(i == 0) _pipelineStateDesc._shaderProgram._vertexShader = resourceManager->GetShader(_shaderID);
							if(i == 1) _pipelineStateDesc._shaderProgram._pixelShader = resourceManager->GetShader(_shaderID);
							if(i == 2) _pipelineStateDesc._shaderProgram._geometryShader = resourceManager->GetShader(_shaderID);
							if(i == 3) _pipelineStateDesc._shaderProgram._hullShader = resourceManager->GetShader(_shaderID);
							if(i == 4) _pipelineStateDesc._shaderProgram._domainShader = resourceManager->GetShader(_shaderID);
							if(i == 5) _pipelineStateDesc._shaderProgram._computeShader = resourceManager->GetShader(_shaderID);
						}
					}
				}

				if (_pipelineStateTable.HasMember(primitiveTopology))
				{
					if (!StringToEnum(_pipelineStateTable[primitiveTopology].GetString(), _pipelineStateDesc._primitiveTopology))
					{
						assert(false);
					}
				}

				if (_pipelineStateTable.HasMember(viewport))
				{
					for (auto& _viewport : _pipelineStateTable[viewport].GetArray())
					{
						// Todo : 어떻게 해야 할까?
					}
				}

				if (_pipelineStateTable.HasMember(scissors))
				{
					for (auto& _scissor : _pipelineStateTable[scissors].GetArray())
					{
						// Todo : 어떻게 해야 할까?
					}
				}

				if (_pipelineStateTable.HasMember(depthDesc))
				{
					auto _depthUUID = _pipelineStateTable[depthDesc].GetString();

					auto _findIt = std::find_if(std::begin(_depthDescMap), std::end(_depthDescMap),
						[&_depthUUID](auto& _pair)
						{
							return (_depthUUID == _pair.first);
						});

					assert(_findIt != _depthDescMap.end());

					_pipelineStateDesc._depthDesc = _findIt->second;
				}
				else
				{

				}

				if (_pipelineStateTable.HasMember(stencilDesc))
				{
					auto _stencilUUID = _pipelineStateTable[stencilDesc].GetString();

					auto _findIt = std::find_if(std::begin(_stencilDescMap), std::end(_stencilDescMap),
						[&_stencilUUID](auto& _pair)
						{
							return (_stencilUUID == _pair.first);
						});

					assert(_findIt != _stencilDescMap.end());

					_pipelineStateDesc._stencilDesc = _findIt->second;
				}
				else
				{
					_pipelineStateDesc._hasDSS = false;
				}

				if (_pipelineStateTable.HasMember(rasterizerDesc))
				{
					auto _rasterizerUUID = _pipelineStateTable[rasterizerDesc].GetString();

					auto _findIt = std::find_if(std::begin(_rasterizerDescMap), std::end(_rasterizerDescMap),
						[&_rasterizerUUID](auto& _pair)
						{
							return (_rasterizerUUID == _pair.first);
						});

					assert(_findIt != _rasterizerDescMap.end());

					_pipelineStateDesc._rasterizerDesc = _findIt->second;
				}
				else
				{
					_pipelineStateDesc._hasRRS = false;
				}

				if (_pipelineStateTable.HasMember(blendDesc))
				{
					auto _blendUUID = _pipelineStateTable[blendDesc].GetString();

					auto _findIt = std::find_if(std::begin(_blendDescMap), std::end(_blendDescMap),
						[&_blendUUID](auto& _pair)
						{
							return (_blendUUID == _pair.first);
						});

					assert(_findIt != _blendDescMap.end());

					_pipelineStateDesc._blendDesc = _findIt->second;
				}
				else
				{
					_pipelineStateDesc._hasBS = false;
				}

				resourceManager->CreatePipelineState(_uuid, _pipelineStateDesc);
			}

			_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/PipelineStateTable.json"));
		}


		return true;
	}

	bool TableLoader::LoadRenderingPipelineTable(ResourceManager* resourceManager)
	{
		auto* _jsonReader = Utility::JsonReader::GetInstance();

		auto _pipelineLayoutTables = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/PipelineLayoutTable.json"));

		const TCHAR* UUID = TEXT("UUID");
		const TCHAR* count = TEXT("Count");

		const TCHAR* description = TEXT("Description");
		const TCHAR* resourceType = TEXT("ResourceType");
		const TCHAR* bindFlags = TEXT("BindFlags");
		const TCHAR* stageFlags = TEXT("StageFlags");
		const TCHAR* slot = TEXT("Slot");
		const TCHAR* arraySize = TEXT("ArraySize");


		auto _layoutArray = _pipelineLayoutTables->GetArray();
		uint32 _resourceCount;

		for (uint32 _tableIdx = 0; _tableIdx < _layoutArray.Size(); _tableIdx += _resourceCount)
		{
			PipelineLayoutDesc _pipelineLayoutDesc;

			uuid _uuid;

			if (_layoutArray[_tableIdx].HasMember(UUID))
			{
				_uuid = _layoutArray[_tableIdx][UUID].GetString();
			}

			if (!_layoutArray[_tableIdx].HasMember(count)) assert(false);

			_resourceCount = _layoutArray[_tableIdx][count].GetInt();

			_pipelineLayoutDesc._bindings.reserve(_resourceCount);

			for (uint32 _resourceIdx = 0; _resourceIdx < _resourceCount; _resourceIdx++)
			{
				uint32 _idx = _tableIdx + _resourceIdx;

				BindingDescriptor _bindingDesc;

				if (_layoutArray[_idx].HasMember(description))
				{
					_bindingDesc._name = StringHelper::WStringToString(_layoutArray[_idx][description].GetString());
				}

				if (_layoutArray[_idx].HasMember(resourceType))
				{
					if (!StringToEnum(_layoutArray[_idx][resourceType].GetString(), _bindingDesc._type))
					{
						assert(false);
					}
				}

				if (_layoutArray[_idx].HasMember(bindFlags))
				{
					uint32 _flags = 0;

					for (auto& _bind : _layoutArray[_idx][bindFlags].GetArray())
					{
						BindFlags::eBindFlags _bindFlag;

						if (StringToEnum(_bind.GetString(), _bindFlag))
						{
							_flags |= _bindFlag;
						}
						else
						{
							assert(false);
						}
					}

					_bindingDesc._bindFlags = _flags;
				}

				if (_layoutArray[_idx].HasMember(stageFlags))
				{
					uint32 _flags = 0;

					for (auto& _stage : _layoutArray[_idx][stageFlags].GetArray())
					{
						StageFlags::eStageFlags _stageFlag;

						if (StringToEnum(_stage.GetString(), _stageFlag))
						{
							_flags |= _stageFlag;
						}
						else
						{
							assert(false);
						}
					}

					_bindingDesc._stageFlags = _flags;
				}

				if (_layoutArray[_idx].HasMember(slot))
				{
					_bindingDesc._slot = _layoutArray[_idx][slot].GetInt();
				}

				if (_layoutArray[_idx].HasMember(arraySize))
				{
					_bindingDesc._arraySize = _layoutArray[_idx][arraySize].GetInt();
				}


				_pipelineLayoutDesc._bindings.push_back(_bindingDesc);
			}

			resourceManager->CreatePipelineLayout(_uuid, _pipelineLayoutDesc);
		}

		_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/PipelineLayoutTable.json"));

		return true;
	}

	bool TableLoader::LoadTextureTable(ResourceManager* resourceManager)
	{
		auto* _jsonReader = Utility::JsonReader::GetInstance();

		auto _textureTables = _jsonReader->LoadJson(TEXT("Asset/GraphicsTable/TextureTable.json"));

		const TCHAR* UUID = TEXT("UUID");
		const TCHAR* textureType = TEXT("TextureType");
		const TCHAR* bindFlags = TEXT("BindFlags");
		const TCHAR* miscFlags = TEXT("MiscFlags");
		const TCHAR* format = TEXT("Format");
		const TCHAR* extend = TEXT("Extend");
		const TCHAR* arrayLayers = TEXT("ArrayLayers");
		const TCHAR* mipLevels = TEXT("MipLevels");
		const TCHAR* samples = TEXT("Samples");

		for (auto& _textureTable : _textureTables->GetArray())
		{
			TextureDesc _textureDesc;
			uuid _uuid;

			if (_textureTable.HasMember(UUID))
			{
				_uuid = _textureTable[UUID].GetString();
			}

			if (_textureTable.HasMember(textureType))
			{
				if (!StringToEnum(_textureTable[textureType].GetString(), _textureDesc._textureType))
				{
					assert(false);
				}
			}

			if (_textureTable.HasMember(bindFlags))
			{
				_textureDesc._bindFlags = 0;
				for (auto& _binds : _textureTable[bindFlags].GetArray())
				{
					BindFlags::eBindFlags _flags;
					if (StringToEnum(_binds.GetString(), _flags))
					{
						_textureDesc._bindFlags |= _flags;
					}
					else
					{
						assert(false);
					}
				}
			}

			if (_textureTable.HasMember(miscFlags))
			{
				_textureDesc._miscFlags = 0;
				for (auto& _misc : _textureTable[miscFlags].GetArray())
				{
					MiscFlags::eMiscFlags _flags;
					if (!StringToEnum(_misc.GetString(), _flags))
					{
						_textureDesc._bindFlags |= _flags;
					}
				}
			}

			if (_textureTable.HasMember(format))
			{
				if (!StringToEnum(_textureTable[format].GetString(), _textureDesc._format))
				{
					assert(false);
				}
			}

			if (_textureTable.HasMember(extend))
			{
				for (uint32 i = 0; i < _textureTable[extend].Size(); i++)
				{
					if (i == 0) _textureDesc._extend._width = _textureTable[extend][i].GetInt();
					if (i == 1) _textureDesc._extend._height = _textureTable[extend][i].GetInt();
					if (i == 2) _textureDesc._extend._depth = _textureTable[extend][i].GetInt();
				}
			}

			if (_textureTable.HasMember(arrayLayers))
			{
				_textureDesc._arrayLayers = _textureTable[arrayLayers].GetInt();
			}

			if (_textureTable.HasMember(mipLevels))
			{
				_textureDesc._mipLevels = _textureTable[mipLevels].GetInt();
			}

			if (_textureTable.HasMember(samples))
			{
				_textureDesc._samples = _textureTable[samples].GetInt();
			}

			resourceManager->CreateTexture(_uuid, _textureDesc);
		}

		_jsonReader->UnloadJson(TEXT("Asset/GraphicsTable/TextureTable.json"));

		return true;
	}
}