#pragma once

#include <algorithm>
#include <type_traits>
#include <string>
#include <memory>

#include "Object.h"
#include "../ID3D.h"


namespace RenderCore
{
	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="Type"></typeparam>
	template<typename Type>
	class IResource : public Object
	{
	public:
		IResource();
		IResource(uint32 bindPoint);
		IResource(uint32 bindPoint, uint32 count);
		IResource(const char* name, uint32 bindPoint, uint32 count);

	protected:

		// Binding Slot Number
		uint32 mBindPoint;

		// Resource Count
		uint32 mCount;

	public:
		uint32 GetBindPoint() const { return mBindPoint; }
		uint32 GetCount() const { return mCount; }
	};

	template<typename Type>
	RenderCore::IResource<Type>::IResource(const char* name, uint32 bindPoint, uint32 count)
		: Object(name)
	{
		mBindPoint = bindPoint;
		mCount = count;
	}

	template<typename Type>
	RenderCore::IResource<Type>::IResource(uint32 bindPoint, uint32 count)
	{
		mBindPoint = bindPoint;
		mCount = count;
	}

	template<typename Type>
	RenderCore::IResource<Type>::IResource(uint32 bindPoint)
	{
		mBindPoint = bindPoint;
	}

	template<typename Type>
	RenderCore::IResource<Type>::IResource()
	{

	}

}