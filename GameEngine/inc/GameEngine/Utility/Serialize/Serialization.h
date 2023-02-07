#pragma once

#include "Common.h"

#include "Math/Math.h"

#include <boost/property_tree/ptree.hpp>


namespace GameEngine
{
	namespace Core
	{
		class Prefab;
	}

	struct UUIDPropertyHolder;

	class Serializable
	{
	public:
		void serialize(GameEngine::Core::Prefab& prefab);
		bool Deserialize(GameEngine::Core::Prefab& prefab, uuid uuid);
	private:
		void serialize(GameEngine::Core::Prefab& prefab, boost::property_tree::ptree& pt);
		bool Deserialize(GameEngine::Core::Prefab& prefab, boost::property_tree::ptree& pt);

		void SetUUIDProperty(GameEngine::Core::Prefab& prefab);

		void SetObject(GameEngine::Core::Prefab& prefab, std::shared_ptr<UUIDPropertyHolder>& holder);
		void SetComponent(GameEngine::Core::Prefab& prefab, std::shared_ptr<UUIDPropertyHolder>& holder);
		void SetResource(std::shared_ptr<UUIDPropertyHolder>& holder);

		std::vector<std::shared_ptr<UUIDPropertyHolder>> m_UUIDHolders;
	};

	template<typename T>
	void serializeConfig(T&, std::string&, boost::property_tree::ptree& pt)
	{
		static_assert(false, "need template specialization for T");
	}

	template<typename T>
	T parseConfig(boost::property_tree::ptree& pt)
	{
		static_assert(false, "need template specialization for T");
	}
}