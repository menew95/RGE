#include "JsonReader/JsonReader.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/error/en.h"
#include "rapidjson/writer.h"
#include "rapidjson/OStreamWrapper.h"

#include <fstream>

#pragma once

namespace Utility
{
	inline void JSON_ASSERT(const TCHAR* text, ...)
	{
		TCHAR buffer[64];

		int result;
		va_list args;

		va_start(args, text);
		result = _snwprintf_s(buffer, 64, text, args);
		va_end(args);

		throw MessageBox(nullptr, buffer, L"Error", MB_OK);
	}

	std::unique_ptr<Utility::JsonReader, JsonReader::Deleter> JsonReader::_instance;
	std::once_flag JsonReader::_flag;

	Utility::JsonReader::JsonReader()
	{
		_documents.reserve(8);
	}

	JsonReader* const Utility::JsonReader::GetInstance()
	{
		std::call_once(JsonReader::_flag, []()
			{
				_instance.reset(new JsonReader);
			});

		return _instance.get();
	}

	void Utility::JsonReader::Release()
	{
		_instance.release();

		return;
	}

	Utility::TDocument* JsonReader::LoadJson(const tstring& jsonPath)
	{
		if (_documents.size() > 0)
		{
			auto foundDoc = _documents.find(jsonPath);

			if (foundDoc != _documents.end())
			{
				return foundDoc->second.get();
			}
		}
		FILE* fp = nullptr;

#if defined(UNICODE)
		errno_t err = _wfopen_s(&fp, jsonPath.c_str(), _T("rb"));
#else
		errno_t err = _fopen_s(&fp_, jsonPath.c_str(), _T("rb"));
#endif

		if (err)
		{
			JSON_ASSERT(_T("JSON file load error : %d\n"), err);
		}

		char readBuffer[23768];

		rapidjson::FileReadStream readStream{ fp, readBuffer, sizeof(readBuffer) };
		rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(readStream);  // wraps bis into eis

		auto document = std::make_unique<Utility::TDocument>();

		rapidjson::ParseResult result = document->ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis);

		if (!result)
		{
			JSON_ASSERT(_T("JSON parse error: %s (%u)\n"), GetParseError_En(result.Code()), result.Offset());
			return nullptr;
		}

		fclose(fp);

		_documents.emplace(jsonPath, std::move(document));

		return _documents.find(jsonPath)->second.get();
	}

	void JsonReader::UnloadJson(const tstring& jsonPath)
	{
		_documents.erase(jsonPath);
	}

	bool JsonReader::WriteJson(const tstring& jsonPath, rapidjson::Document& document)
	{
		std::ofstream ofs{ StringHelper::ToTString(jsonPath) };
		rapidjson::OStreamWrapper  osw{ ofs };
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
		document.Accept(writer);

		ofs.close();

		return true;
	}
}