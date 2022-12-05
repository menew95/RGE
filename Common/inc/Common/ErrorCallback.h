#pragma once

namespace Common
{
	class ErrorCallback
	{
	public:
		virtual void Error(const char* msg) abstract;

		virtual void Waring(const char* msg) abstract;

		virtual void Log(const char* msg) abstract;
	};
}