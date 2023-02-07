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

#include "GameEngine/Core/System/Resources.h"
#include "GameEngine/Core/Resource/Mesh.h"
#include "GameEngine/Core/Resource/Material.h"
#include "GameEngine/Core/Resource/AnimationClip.h"

namespace GameEngine
{
	struct UUIDPropertyHolder
	{
		rttr::property _prop;
		//std::shared_ptr<Core::Component> _variant;
		rttr::variant _variant;
		std::vector<uuid> _uuids;
		Core::ObjectType _objectType;
		Core::ResourceType _resourceType;
	};

	template<>
	void serializeConfig(Math::Vector2& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);

		pt.push_back(std::make_pair(valueName, _vec));
	}

	template<>
	void serializeConfig(Math::Vector3& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);
		_vec.put("z", v.z);

		pt.push_back(std::make_pair(valueName, _vec));
	}

	template<>
	void serializeConfig(Math::Vector4& v, std::string& valueName, boost::property_tree::ptree& pt)
	{
		boost::property_tree::ptree _vec;

		_vec.put("x", v.x);
		_vec.put("y", v.y);
		_vec.put("z", v.z);
		_vec.put("w", v.w);

		pt.push_back(std::make_pair(valueName, _vec));
	}


	template<>
	void serializeConfig(Math::Matrix& m, std::string& valueName, boost::property_tree::ptree& pt)
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

				serializeConfig(_v, _propName, pt);
				break;
			}
			case Core::MetaDataType::VECTOR3:
			{
				auto _v = var.convert<Math::Vector3>();

				serializeConfig(_v, _propName, pt);
				break;
			}
			case Core::MetaDataType::VECTOR4:
			{
				auto _v = var.convert<Math::Vector4>();

				serializeConfig(_v, _propName, pt);
				break;
			}
			case Core::MetaDataType::MATRIX:
			{
				auto _m = var.convert<Math::Matrix>();

				serializeConfig(_m, _propName, pt);
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

	template<>
	Math::Vector2 parseConfig(boost::property_tree::ptree& pt)
	{
		Math::Vector2 _data;

		_data.x = pt.get<float>("x");
		_data.y = pt.get<float>("y");

		return _data;
	}

	template<>
	Math::Vector3 parseConfig(boost::property_tree::ptree& pt)
	{
		Math::Vector3 _data;

		_data.x = pt.get<float>("x");
		_data.y = pt.get<float>("y");
		_data.z = pt.get<float>("z");

		return _data;
	}

	template<>
	Math::Vector4 parseConfig(boost::property_tree::ptree& pt)
	{
		Math::Vector4 _data;

		_data.x = pt.get<float>("x");
		_data.y = pt.get<float>("y");
		_data.z = pt.get<float>("z");
		_data.w = pt.get<float>("w");

		return _data;
	}

	template<>
	Math::Matrix parseConfig(boost::property_tree::ptree& pt)
	{
		Math::Matrix _data;

		_data._11 = pt.get<float>("_11"); _data._12 = pt.get<float>("_12"); _data._13 = pt.get<float>("_13");  _data._14 = pt.get<float>("_14");
		_data._21 = pt.get<float>("_21"); _data._22 = pt.get<float>("_22"); _data._23 = pt.get<float>("_23");  _data._24 = pt.get<float>("_24");
		_data._31 = pt.get<float>("_31"); _data._32 = pt.get<float>("_32"); _data._33 = pt.get<float>("_33");  _data._34 = pt.get<float>("_34");
		_data._41 = pt.get<float>("_41"); _data._42 = pt.get<float>("_42"); _data._43 = pt.get<float>("_43");  _data._44 = pt.get<float>("_44");

		return _data;
	}

	void SetProperty(GameEngine::Core::MetaDataType type, const rttr::type& component_type, const rttr::variant& var
		, rttr::property& prop, boost::property_tree::ptree& pt, std::vector<std::shared_ptr<UUIDPropertyHolder>>& holders)
	{
		try{
			switch (type)
			{
				case GameEngine::Core::MetaDataType::WSTRING:
				{
					auto _data = pt.get<std::string>("");
					auto _tstring = StringHelper::StringToWString(_data);

					prop.set_value(var, _tstring);

					break;
				}
				case GameEngine::Core::MetaDataType::VECTOR2:
				{
					Math::Vector2 _data = parseConfig<Math::Vector2>(pt);

					prop.set_value(var, _data);

					break;
				}
				case GameEngine::Core::MetaDataType::VECTOR3:
				{
					Math::Vector3 _data = parseConfig<Math::Vector3>(pt);

					prop.set_value(var, _data);

					break;
				}
				case GameEngine::Core::MetaDataType::VECTOR4:
				{
					Math::Vector4 _data = parseConfig<Math::Vector4>(pt);

					prop.set_value(var, _data);

					break;
				}
				case GameEngine::Core::MetaDataType::MATRIX:
				{
					Math::Matrix _data = parseConfig<Math::Matrix>(pt);

					prop.set_value(var, _data);

					break;
				}
				case GameEngine::Core::MetaDataType::UUID:
				{
					// UUID에 해당하는 리소스 또는 오브젝트 연결이 필요함
					// 리소스의 경우에는 프리팹 로드를 제일 마지막에 하면 Resources 에서 로드가 가능하겠지만
					// 만약 Transform 에 parent 를 연결해야 한다고 칠때 해당 uuid 를 가진 오브젝트가 아직 생성이 안되었을 경우 연결 해 줄 수가 없음
					// 미리 어딘가에 보관하고 오브젝트들이 모두 생성이 된후에 연결이 필요해보임

					auto _data = pt.get<std::string>("");
					auto _tstring = StringHelper::StringToWString(_data);


					rttr::variant _metaVariant = prop.get_metadata(GameEngine::Core::MetaData::ObjectType);

					if (!_metaVariant.is_valid())
						return;

					GameEngine::Core::ObjectType _objectType = _metaVariant.get_value<GameEngine::Core::ObjectType>();

					UUIDPropertyHolder _holder
					{
						_holder._prop = prop,
					};

					_holder._variant = var;
					_holder._uuids.push_back(_tstring);
					_holder._objectType = _objectType;

					switch (_objectType)
					{
						case GameEngine::Core::ObjectType::GameObject:
							break;
						case GameEngine::Core::ObjectType::Component:
						{
							rttr::variant _comMeta = prop.get_metadata(GameEngine::Core::ObjectType::Component);

							if (!_comMeta.is_valid())
								return;

							GameEngine::Core::ComponentType _componentType = _comMeta.get_value<GameEngine::Core::ComponentType>();
							break;
						}
						case GameEngine::Core::ObjectType::Resource:
						{
							rttr::variant _resMeta = prop.get_metadata(GameEngine::Core::ObjectType::Resource);

							if (!_resMeta.is_valid())
								return;

							GameEngine::Core::ResourceType _resourceType = _resMeta.get_value<GameEngine::Core::ResourceType>();

							_holder._resourceType = _resourceType;
							break;
						}
						default:
							break;
					}

					holders.push_back(std::make_shared<UUIDPropertyHolder>(_holder));
					break;
				}
				case GameEngine::Core::MetaDataType::BOOL:
				{
					auto _data = pt.get<bool>("");

					prop.set_value(var, _data);

					break;
				}
				case GameEngine::Core::MetaDataType::UINT32:
				{
					auto _data = pt.get<uint32>("");

					prop.set_value(var, _data);

					break;
				}
				case GameEngine::Core::MetaDataType::INT32:
				{
					auto _data = pt.get<int32>("");

					prop.set_value(var, _data);

					break;
				}
				case GameEngine::Core::MetaDataType::FLOAT:
				{
					auto _data = pt.get<float>("");

					prop.set_value(var, _data);

					break;
				}
				default:
					assert(false);
					break;
			}
		}
		catch (const boost::property_tree::json_parser_error& e)
		{
			auto error = e.what();
		}
	}

		void SetPropertyArray(GameEngine::Core::MetaDataType type, const rttr::type& component_type, const rttr::variant& var
			, rttr::property& prop, boost::property_tree::ptree& pt, std::vector<std::shared_ptr<UUIDPropertyHolder>>& holders)
		{
			try {
				switch (type)
				{
					case GameEngine::Core::MetaDataType::WSTRING:
					{
						std::vector<tstring> _data;

						for (auto& kv : pt)
						{
							auto _value = kv.second.get<std::string>("");
							auto _tstring = StringHelper::StringToWString(_value);

							_data.push_back(_tstring);
						}

						prop.set_value(var, _data);
						break;
					}
					case GameEngine::Core::MetaDataType::VECTOR2:
					{
						std::vector<Math::Vector2> _data;

						for (auto& kv : pt)
						{
							auto _value = parseConfig<Math::Vector2>(kv.second);

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					case GameEngine::Core::MetaDataType::VECTOR3:
					{
						std::vector<Math::Vector3> _data;

						for (auto& kv : pt)
						{
							auto _value = parseConfig<Math::Vector3>(kv.second);

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					case GameEngine::Core::MetaDataType::VECTOR4:
					{
						std::vector<Math::Vector4> _data;

						for (auto& kv : pt)
						{
							auto _value = parseConfig<Math::Vector4>(kv.second);

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					case GameEngine::Core::MetaDataType::MATRIX:
					{
						std::vector<Math::Matrix> _data;

						for (auto& kv : pt)
						{
							auto _value = parseConfig<Math::Matrix>(kv.second);

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					case GameEngine::Core::MetaDataType::UUID:
					{
						// UUID에 해당하는 리소스 또는 오브젝트 연결이 필요함
						// 리소스의 경우에는 프리팹 로드를 제일 마지막에 하면 Resources 에서 로드가 가능하겠지만
						// 만약 Transform 에 parent 를 연결해야 한다고 칠때 해당 uuid 를 가진 오브젝트가 아직 생성이 안되었을 경우 연결 해 줄 수가 없음
						// 미리 어딘가에 보관하고 오브젝트들이 모두 생성이 된후에 연결이 필요해보임

						std::vector<uuid> _data;

						for (auto& kv : pt)
						{
							auto _value = kv.second.get<std::string>("");
							auto _tstring = StringHelper::StringToWString(_value);

							_data.push_back(_tstring);
						}


						rttr::variant _metaVariant = prop.get_metadata(GameEngine::Core::MetaData::ObjectType);

						if (!_metaVariant.is_valid())
							return;

						GameEngine::Core::ObjectType _objectType = _metaVariant.get_value<GameEngine::Core::ObjectType>();

						//std::shared_ptr<GameEngine::Core::Component> _target = var.get_value<std::shared_ptr<Core::Component>>();

						UUIDPropertyHolder _holder
						{
							_holder._prop = prop
						};

						_holder._variant = var;
						_holder._uuids = _data;
						_holder._objectType = _objectType;

						switch (_objectType)
						{
							case GameEngine::Core::ObjectType::GameObject:
								break;
							case GameEngine::Core::ObjectType::Component:
							{
								rttr::variant _comMeta = prop.get_metadata(GameEngine::Core::ObjectType::Component);

								if (!_comMeta.is_valid())
									return;

								GameEngine::Core::ComponentType _componentType = _comMeta.get_value<GameEngine::Core::ComponentType>();
								break;
							}
							case GameEngine::Core::ObjectType::Resource:
							{
								rttr::variant _resMeta = prop.get_metadata(GameEngine::Core::ObjectType::Resource);

								if (!_resMeta.is_valid())
									return;

								GameEngine::Core::ResourceType _resourceType = _resMeta.get_value<GameEngine::Core::ResourceType>();

								_holder._resourceType = _resourceType;
								break;
							}
							default:
								break;
						}
						// uuid 에 해당하는 오브젝트(리소스 등)
						// 일단 uuid 로 오브젝트를 넣어줘야하는 것들을 (target component, uuid, type?)들을 모아뒀다가 나중에 한번에 찾아야겠음


						holders.push_back(std::make_shared<UUIDPropertyHolder>(_holder));
						break;
					}
					case GameEngine::Core::MetaDataType::BOOL:
					{
						std::vector<bool> _data;

						for (auto& kv : pt)
						{
							auto _value = kv.second.get<bool>("");

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					case GameEngine::Core::MetaDataType::UINT32:
					{
						std::vector<uint32> _data;

						for (auto& kv : pt)
						{
							auto _value = kv.second.get<uint32>("");

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					case GameEngine::Core::MetaDataType::INT32:
					{
						std::vector<int32> _data;

						for (auto& kv : pt)
						{
							auto _value = kv.second.get<int32>("");

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					case GameEngine::Core::MetaDataType::FLOAT:
					{
						std::vector<float> _data;

						for (auto& kv : pt)
						{
							auto _value = kv.second.get<float>("");

							_data.push_back(_value);
						}

						prop.set_value(var, _data);

						break;
					}
					default:
						assert(false);
						break;
				}
			}
			catch (const boost::property_tree::json_parser_error& e)
			{
				auto error = e.what();
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

			_go->SetName(_goName);

			for (const auto& child : node.second)
			{
				auto _key = child.first;

				rttr::type _comType = rttr::type::get_by_name(_key);

				rttr::constructor _ctor = _comType.get_constructor(
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
				_go->AddComponent(component);
				for (const auto& grandchild : child.second)
				{
					if (grandchild.first == "<xmlattr>")
						continue;

					auto _grandKey = grandchild.first;

					rttr::property _property = _comType.get_property(_grandKey);

					//const rttr::variant value = _property.get_value(comp);


					if (_property.is_valid())
					{
						auto _meta = _property.get_metadata(GameEngine::Core::MetaData::Serializable);

						if(!_meta.is_valid())
							continue;

						assert(_meta.can_convert<GameEngine::Core::MetaDataType>() == true);

						GameEngine::Core::MetaDataType _metaDataType = _meta.get_value<GameEngine::Core::MetaDataType>();

						auto _grandpt = grandchild.second;

						if (_property.get_type().is_sequential_container()/*grandchild.second.count("") > 1*/)
						{
							// this is array value

							SetPropertyArray(_metaDataType, _comType, _comVar, _property, _grandpt, m_UUIDHolders);
						}
						else
						{
							SetProperty(_metaDataType, _comType, _comVar, _property, _grandpt, m_UUIDHolders);
						}
					}
				}
			}

			prefab.RegistGameObject(_go);
		}

		SetUUIDProperty(prefab);

		return true;
	}

	void Serializable::SetUUIDProperty(GameEngine::Core::Prefab& prefab)
	{
		for (size_t _i = 0; _i < m_UUIDHolders.size(); _i++)
		{
			switch (m_UUIDHolders[_i]->_objectType)
			{
				case GameEngine::Core::ObjectType::GameObject:
				{
					SetObject(prefab, m_UUIDHolders[_i]);
					break;
				}
				case GameEngine::Core::ObjectType::Component:
				{
					SetComponent(prefab, m_UUIDHolders[_i]);
					break;
				}
				case GameEngine::Core::ObjectType::Resource:
				{
					SetResource(m_UUIDHolders[_i]);
					break;
				}
				default:
					assert(false);
					break;
			}
		}
	}

	void Serializable::SetObject(GameEngine::Core::Prefab& prefab, std::shared_ptr<UUIDPropertyHolder>& holder)
	{
		using namespace GameEngine::Core;

		assert(false);
	}

	void Serializable::SetComponent(GameEngine::Core::Prefab& prefab, std::shared_ptr<UUIDPropertyHolder>& holder)
	{
		using namespace GameEngine::Core;

		if (holder->_prop.get_type().is_sequential_container())
		{
			std::vector<std::shared_ptr<Component>> _objects;

			for (auto& _uuid : holder->_uuids)
			{
				auto _gameObject = prefab.FindGameObject(_uuid);

				if (_gameObject != nullptr)
				{
					rttr::variant _comMeta = holder->_prop.get_metadata(GameEngine::Core::ObjectType::Component);

					if (!_comMeta.is_valid())
						return;

					std::string _value = _comMeta.get_value<std::string>();

					tstring _componentType = StringHelper::StringToWString(_value);

					auto _findCom = _gameObject->GetComponent(_componentType);

					if (_findCom != nullptr)
					{
						_objects.push_back(_findCom);
					}
				}
				else
					assert(false);
			}

			rttr::variant _var = _objects;

			if (!_var.convert(holder->_prop.get_type()))
				return;

			if (!holder->_prop.set_value(holder->_variant, _var))
				assert(false);
		}
		else
		{
			assert(holder->_uuids.size() == 1);

			auto _gameObject = prefab.FindGameObject(holder->_uuids[0]);

			if (_gameObject == nullptr)
				assert(false);

			rttr::variant _comMeta = holder->_prop.get_metadata(GameEngine::Core::ObjectType::Component);

			if (!_comMeta.is_valid())
				return;

			std::string _value = _comMeta.get_value<std::string>();

			tstring _componentType = StringHelper::StringToWString(_value);

			auto _findCom = _gameObject->GetComponent(_componentType);

			rttr::variant _var = _findCom;

			if (!_var.convert(holder->_prop.get_type()))
				return;

			if (!holder->_prop.set_value(holder->_variant, _var))
				assert(false);
		}
	}

	void Serializable::SetResource(std::shared_ptr<UUIDPropertyHolder>& holder)
	{
		using namespace GameEngine::Core;

		switch (holder->_resourceType)
		{
			case GameEngine::Core::ResourceType::Mesh:
			{
				if (holder->_prop.get_type().is_sequential_container())
				{
					std::vector<std::shared_ptr<Mesh>> _resources;

					for (auto& _uuid : holder->_uuids)
					{
						auto _resource = Resources::GetInstance()->GetResource<Mesh>(_uuid);

						if (_resource != nullptr)
						{
							_resources.push_back(_resource);
						}
					}

					holder->_prop.set_value(holder->_variant, _resources);
				}
				else
				{
					assert(holder->_uuids.size() == 1);

					auto _resource = Resources::GetInstance()->GetResource<Mesh>(holder->_uuids[0]);

					if (_resource != nullptr)
					{
						holder->_prop.set_value(holder->_variant, _resource);
					}
				}
				break;
			}
			case GameEngine::Core::ResourceType::Bone:
			{
				assert(false);
				/*if (holder->_uuids.size() > 1)
				{
					std::vector<std::shared_ptr<Mesh>> _resources;

					for (auto& _uuid : holder->_uuids)
					{
						auto _resource = Resources::GetInstance()->GetResource<BoneData>(_uuid);

						if (_resource != nullptr)
						{
							_resources.push_back(_resource);
						}
					}

					holder->_prop.set_value(holder->_variant, _resources);
				}
				else
				{
					assert(holder->_uuids.size() == 1);

					auto _resource = Resources::GetInstance()->GetResource<BoneData>(holder->_uuids[0]);

					if (_resource != nullptr)
					{
						holder->_prop.set_value(holder->_variant, _resource);
					}
				}*/
				break;
			}
			case GameEngine::Core::ResourceType::Material:
			{
				if (holder->_prop.get_type().is_sequential_container())
				{
					std::vector<std::shared_ptr<Material>> _resources;

					for (auto& _uuid : holder->_uuids)
					{
						auto _resource = Resources::GetInstance()->GetResource<Material>(_uuid);

						if (_resource != nullptr)
						{
							_resources.push_back(_resource);
						}
					}

					holder->_prop.set_value(holder->_variant, _resources);
				}
				else
				{
					assert(holder->_uuids.size() == 1);

					auto _resource = Resources::GetInstance()->GetResource<Material>(holder->_uuids[0]);

					if (_resource != nullptr)
					{
						holder->_prop.set_value(holder->_variant, _resource);
					}
				}
				break;
			}
			case GameEngine::Core::ResourceType::AnimationClip:
			{
				if (holder->_prop.get_type().is_sequential_container())
				{
					std::vector<std::shared_ptr<AnimationClip>> _resources;

					for (auto& _uuid : holder->_uuids)
					{
						auto _resource = Resources::GetInstance()->GetResource<AnimationClip>(_uuid);

						if (_resource != nullptr)
						{
							_resources.push_back(_resource);
						}
					}

					holder->_prop.set_value(holder->_variant, _resources);
				}
				else
				{
					assert(holder->_uuids.size() == 1);

					auto _resource = Resources::GetInstance()->GetResource<AnimationClip>(holder->_uuids[0]);

					if (_resource != nullptr)
					{
						holder->_prop.set_value(holder->_variant, _resource);
					}
				}
				break;
			}
			case GameEngine::Core::ResourceType::AudioClip:
			{
				assert(false);
				/*if (holder->_uuids.size() > 1)
				{
					std::vector<std::shared_ptr<Material>> _resources;

					for (auto& _uuid : holder->_uuids)
					{
						auto _resource = Resources::GetInstance()->GetResource<Material>(_uuid);

						if (_resource != nullptr)
						{
							_resources.push_back(_resource);
						}
					}

					holder->_prop.set_value(holder->_variant, _resources);
				}
				else
				{
					assert(holder->_uuids.size() == 1);

					auto _resource = Resources::GetInstance()->GetResource<Mesh>(holder->_uuids[0]);

					if (_resource != nullptr)
					{
						holder->_prop.set_value(holder->_variant, _resource);
					}
				}*/
				break;
			}
			default:
				assert(false);
				break;
		}
	}

}