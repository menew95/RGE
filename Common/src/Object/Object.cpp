#include "Object/Object.h"

#include "Common/StringHelper.h"
#include "uuid.h"

tstring UUIDGenerator::Generate()
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size>{};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);
	uuids::uuid_random_generator gen{ generator };

	const uuids::uuid id = gen();
	const tstring uuidString = StringHelper::ToTString(uuids::to_string(id));
	return uuidString;
}

Object::Object(const tstring& _typeName) :
	Object(_typeName, UUIDGenerator::Generate()) { }

Object::Object(const tstring& _typeName, const tstring& _uuid) :
	Object(_typeName, _uuid, StringHelper::Format(TEXT("%s {%s}"), _typeName.c_str(), _uuid.c_str())) { }

Object::Object(const tstring& _typeName, const tstring& _uuid, const tstring& _name) :
	m_TypeName(_typeName),
	m_UUID(_uuid),
	m_Name(_name) { }