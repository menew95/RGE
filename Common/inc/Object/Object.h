#pragma once

#include "Common.h"

#include "Common/StringHelper.h"

class UUIDGenerator
{
public:
	static tstring Generate();
};


#ifdef COMMON_EXPORT
#ifndef COMMON
#define COMMON dll_export
#endif
#else
#ifndef COMMON
#define COMMON dll_import
#endif
#endif

class COMMON Object
{
public:
	Object() = default;

	Object(const tstring& _typeName);

	Object(const tstring& _typeName, const tstring& _uuid);

	Object(const tstring& _typeName, const tstring& _uuid, const tstring& _name);

	tstring m_Name;

	uuid m_UUID;

	tstring m_TypeName;

	std::string m_rawTypeName;

	[[nodiscard]] tstring GetName() const { return m_Name; }

	[[nodiscard]] const uuid& GetUUID() const { return m_UUID; }

	[[nodiscard]] tstring GetTypeName() const { return m_TypeName; }

	void SetName(const tstring& _name) { m_Name = _name; }

	void SetUUID(const uuid& _uuid) { m_UUID = _uuid; }

};