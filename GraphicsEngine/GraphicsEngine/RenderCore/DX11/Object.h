#pragma once

#include <string>

#include "Prefix.h"
#include "Types.h"

namespace RenderCore
{
	class Object
	{
	public:
		Object();
		Object(const char* name);
		Object(const Object&) = default;
		Object(Object&&) = default;
		virtual ~Object() {};

	private:
		// Resource Name
		std::string mName;


	public:
		std::string& GetName() { return mName; }

	};
}