#pragma once
#include <memory>
#include <mutex>
#include <unordered_map>

#include "Common.h"

#include "JsonReader/Export.h"
#include "JsonReader/JsonMacro.h"

#include "rapidjson/document.h"

/**
 * ∞ÊπŒ¿Ã≥◊≤® »…√≥ø»
*/

namespace Utility
{
	using JsonUnicode = rapidjson::UTF8<TCHAR>;
	using TDocument = rapidjson::GenericDocument<JsonUnicode>;

	class JSON_DLL_DECLSPEC JsonReader
	{
	protected:
		JsonReader();
		~JsonReader() = default;

	public:
		JsonReader(const JsonReader& other) = delete;
		JsonReader& operator =(const JsonReader& other) = delete;

	public:
		struct JSON_DLL_DECLSPEC Deleter
		{
			void operator()(JsonReader* instance)
			{
				delete instance;
			}
		};

	private:
		static std::unique_ptr<JsonReader, Deleter> _instance;

		static std::once_flag _flag;

		std::unordered_map<tstring, std::unique_ptr<Utility::TDocument>> _documents;

	public:
		static  JsonReader* const GetInstance();

		static void Release();

	public:
		Utility::TDocument* LoadJson(const tstring& jsonPath);

		void UnloadJson(const tstring& jsonPath);
		bool WriteJson(const tstring& jsonPath, rapidjson::Document& document);
	};

}