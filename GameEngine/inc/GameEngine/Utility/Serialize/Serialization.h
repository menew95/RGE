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

	class Serializable
	{
	public:
		void serialize(GameEngine::Core::Prefab& prefab);
		bool Deserialize(GameEngine::Core::Prefab& prefab, uuid uuid);
	private:
		void serialize(GameEngine::Core::Prefab& prefab, boost::property_tree::ptree& pt);
		bool Deserialize(GameEngine::Core::Prefab& prefab, boost::property_tree::ptree& pt);

	};

	template<typename T>
	void parseConfig(T&, std::string&, boost::property_tree::ptree& pt)
	{
		static_assert(false, "need template specialization for T");
	}
}