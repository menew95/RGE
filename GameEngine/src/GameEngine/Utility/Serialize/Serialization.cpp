#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Utility/Serialize/Serialization.h"

#include "GameEngine/Core/GameObject.h"
#include "GameEngine/Core/Resource/Prefab.h"

#include "Common/StringHelper.h"

#include <rttr/registration>
#include <rttr/type>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "GameEngine/Core/MetaDataInfo.h"

#include "GameEngine/Core/GameObject.h"
#include "GameEngine/Core/Component/Component.h"
#include "GameEngine/Core/Component/Transform.h"

namespace GameEngine
{
	template<>
	void parseConfig(Math::Vector2& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);

		pt.push_back(std::make_pair(valueName, _vec));
	}

	template<>
	void parseConfig(Math::Vector3& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);
		_vec.put("z", v.z);

		pt.push_back(std::make_pair(valueName, _vec));
	}

	template<>
	void parseConfig(Math::Vector4& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);
		_vec.put("z", v.z);
		_vec.put("w", v.w);

		pt.push_back(std::make_pair(valueName, _vec));
	}


	template<>
	void parseConfig(Math::Matrix& m, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _matrix;

		_matrix.put("_11", m._11); _matrix.put("_12", m._12); _matrix.put("_13", m._13); _matrix.put("_14", m._14);
		_matrix.put("_21", m._21); _matrix.put("_22", m._22); _matrix.put("_23", m._23); _matrix.put("_24", m._24);
		_matrix.put("_31", m._31); _matrix.put("_32", m._32); _matrix.put("_33", m._33); _matrix.put("_34", m._34);
		_matrix.put("_41", m._41); _matrix.put("_42", m._42); _matrix.put("_43", m._43); _matrix.put("_44", m._44);

		pt.push_back(std::make_pair(valueName, _matrix));
	}

	void GetProperty(GameEngine::Core::MetaDataType type, const std::shared_ptr<GameEngine::Core::Component>& instance
		, const rttr::type& component_type, const rttr::variant& var
		, rttr::property& prop, boost::property_tree::ptree& pt, bool isArray = false)
	{
		std::string _propName = prop.get_name().to_string();

		switch (type)
		{
			case Core::MetaDataType::WSTRING:
			{
				auto _wString = var.convert<tstring>();
				auto _string = StringHelper::WStringToString(_wString);
				pt.put(_propName, _string);
				break;
			}
			case Core::MetaDataType::VECTOR2:
			{
				auto _v = var.convert<Math::Vector2>();

				parseConfig(_v, _propName, pt);
				break;
			}
			case Core::MetaDataType::VECTOR3:
			{
				auto _v = var.convert<Math::Vector3>();

				parseConfig(_v, _propName, pt);
				break;
			}
			case Core::MetaDataType::VECTOR4:
			{
				auto _v = var.convert<Math::Vector4>();

				parseConfig(_v, _propName, pt);
				break;
			}
			case Core::MetaDataType::MATRIX:
			{
				auto _m = var.convert<Math::Matrix>();

				parseConfig(_m, _propName, pt);
				break;
			}
			case Core::MetaDataType::UUID:
			{
				rttr::variant _vaild = prop.get_metadata(GameEngine::Core::Util::Check_Vaild);

				if (!_vaild.is_valid())
					break;

				auto _vaildMethod = component_type.get_method(_vaild.to_string());

				if (!_vaildMethod.invoke(instance).to_bool()) // 유효하지 않음
					break;

				rttr::variant _get = prop.get_metadata(GameEngine::Core::MetaDataType::UUID);

				if (!_get.is_valid())
					break;

				auto _method = component_type.get_method(_get.to_string());

				auto _mName = _get.to_string();

				if (!_method.is_valid())
					break;

				auto vartype = var.get_type();

				auto _value = _method.invoke(var);

				if (_value.can_convert<uuid>())
				{
					auto _uuid = _value.convert<uuid>();
					auto _stringUUID = StringHelper::WStringToString(_uuid);

					if (isArray)
					{
						pt.put("", _stringUUID);
					}
					else
					{
						pt.put(prop.get_name().to_string(), _stringUUID);
					}
				}

				break;
			}
			case Core::MetaDataType::BOOL:
			{
				auto _bool = var.to_bool();

				pt.put(prop.get_name().to_string(), _bool);
				break;
			}
			case Core::MetaDataType::UINT32:
			{
				auto _u32 = var.to_uint32();

				pt.put(prop.get_name().to_string(), _u32);
				break;
			}
			case Core::MetaDataType::INT32:
			{
				auto _i32 = var.to_int32();

				pt.put(prop.get_name().to_string(), _i32);
				break;
			}
			case Core::MetaDataType::FLOAT:
			{
				auto _f = var.to_float();

				pt.put(prop.get_name().to_string(), _f);
				break;
			}
		}
	}

	void Serializable::serialize(GameEngine::Core::Prefab& prefab, boost::property_tree::ptree& pt)
	{
		for (const auto& go : prefab.GetGameObjectList())
		{

			boost::property_tree::ptree components;
			for (const auto& comp : go->GetComponents())
			{
				boost::property_tree::ptree component;

				const rttr::type component_type = rttr::type::get_by_name(StringHelper::ToString(comp->GetTypeName()));

				for (rttr::property prop : component_type.get_properties())
				{
					const rttr::variant value = prop.get_value(comp);

					rttr::variant _metaVariant = prop.get_metadata(GameEngine::Core::MetaData::Serializable);

					if(!_metaVariant.is_valid())
						continue;

					GameEngine::Core::MetaDataType _metaDataType = _metaVariant.get_value<GameEngine::Core::MetaDataType>();
					
					if (prop.get_type().is_array())
					{
						continue;
					}

					if (value.is_sequential_container())
					{
						auto _seq = value.create_sequential_view();
						boost::property_tree::ptree _array;

						std::cout << _seq.get_size() << std::endl;  // prints: '5'
						for (const auto& item : _seq) // iterates over all elements of the std::vector<T>
						{
							boost::property_tree::ptree _item;

							auto _extractVar = item.extract_wrapped_value();
							GetProperty(_metaDataType, comp, component_type, _extractVar, prop, _item, true);

							_array.push_back(std::make_pair("", _item));
						}

						component.push_back(std::make_pair(prop.get_name().to_string(), _array));
					}
					else
					{
						GetProperty(_metaDataType, comp, component_type, value, prop, component);
					}
				}

				components.push_back(std::make_pair(StringHelper::ToString(comp->GetTypeName()), component));
			}

			auto _goName = StringHelper::WStringToString(go->GetName());

			pt.push_back(std::make_pair(_goName, components));
		}
	}

	void Serializable::serialize(GameEngine::Core::Prefab& prefab)
	{
		boost::property_tree::ptree pt;
		serialize(prefab, pt);

		std::ofstream file("Asset/Prefab/" + StringHelper::WStringToString(prefab.GetName()) + ".meta");
		if (file.is_open())
		{
			boost::property_tree::write_json(file, pt);
		}
		file.close();

	}

	bool Serializable::Deserialize(GameEngine::Core::Prefab& prefab, uuid uuid)
	{
		boost::property_tree::ptree pt;
		std::ifstream file("Asset/Prefab/" + StringHelper::WStringToString(uuid) + ".meta");

		if (!file.good())
			return false;

		try
		{
			boost::property_tree::read_json(file, pt);
		}
		catch (const boost::property_tree::json_parser_error& e)
		{
			auto error = e.what();
		}

		Deserialize(prefab, pt);

		file.close();

		return true;
	}

	bool Serializable::Deserialize(GameEngine::Core::Prefab& prefab, boost::property_tree::ptree& pt)
	{
		for (const auto& node : pt)
		{
			tstring _goName = StringHelper::StringToWString(node.first);

			std::shared_ptr<Core::GameObject> _go = Core::GameObject::Create();

			for (const auto& child : node.second)
			{
				auto _key = child.first;

				auto _comType = rttr::type::get_by_name(_key);

				auto _ctor = _comType.get_constructor(
					{
						rttr::type::get<std::shared_ptr<Core::GameObject>&>(),
						rttr::type::get<const tstring&>()
					});

				rttr::variant _comVar = _comType.create();

				tstring _typeName = StringHelper::StringToWString(_key);

				_comVar = _ctor.invoke(_go, _typeName);

				if (!_comVar.is_valid() && _comVar.can_convert<std::shared_ptr<Core::Component>>())
					break;

				auto component = _comVar.get_value<std::shared_ptr<Core::Component>>();
				_go->AddComponent(component.get());
				for (const auto& grandchild : child.second)
				{
					if (grandchild.first == "<xmlattr>")
						continue;

					auto _grandKey = grandchild.first;
				}
			}

			prefab.RegistGameObject(_go);
		}

		return true;
	}

}