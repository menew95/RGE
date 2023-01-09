

#include "Common.h"
#include "GraphicsModule/Utility/Export.h"
#include <algorithm>
#include <type_traits>
#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <functional>
#include <cstdint>

namespace Graphics
{

    // Alternative to std::make_unique<T> for strict C++11 support.
    template <typename T, typename... Args>
    std::unique_ptr<T> MakeUnique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    // Alternative to std::make_unique<T[]> for strict C++11 support.
    template <typename T>
    std::unique_ptr<T[]> MakeUniqueArray(std::size_t size)
    {
        return std::unique_ptr<T[]>(new T[size]);
    }

    template <class T>
    void InitMemory(T& data)
    {
        static_assert(!std::is_pointer<T>::value, "'InitMemory' does not allow pointer types");
        static_assert(std::is_pod<T>::value, "'InitMemory' does only allow plain-old-data (POD)");
        ::memset(&data, 0, sizeof(T));
    }

    template <class Container, class T>
    void Fill(Container& cont, const T& value)
    {
        std::fill(std::begin(cont), std::end(cont), value);
    }

    template <class Container, class T>
    bool Contains(const Container& cont, const T& value)
    {
        return (std::find(std::begin(cont), std::end(cont), value) != std::end(cont));
    }

    template <class Container, class T>
    void RemoveFromList(Container& cont, const T& entry)
    {
        auto it = std::find(std::begin(cont), std::end(cont), entry);
        if (it != std::end(cont))
            cont.erase(it);
    }

    template <class Container, class UnaryPredicate>
    void RemoveFromListIf(Container& cont, UnaryPredicate pred)
    {
        auto it = std::find_if(std::begin(cont), std::end(cont), pred);
        if (it != std::end(cont))
            cont.erase(it);
    }

    template <class Container, class T>
    void RemoveAllFromList(Container& cont, const T& entry)
    {
        cont.erase(
            std::remove(std::begin(cont), std::end(cont), entry),
            cont.end()
        );
    }

    template <class Container, class UnaryPredicate>
    void RemoveAllFromListIf(Container& cont, UnaryPredicate pred)
    {
        cont.erase(
            std::remove_if(std::begin(cont), std::end(cont), pred),
            cont.end()
        );
    }

    template <class Container, typename T>
    void AddOnceToSharedList(Container& cont, const std::shared_ptr<T>& entry)
    {
        if (entry && std::find(cont.begin(), cont.end(), entry) == cont.end())
            cont.push_back(entry);
    }

    template <class Container, typename T>
    void RemoveFromSharedList(Container& cont, const T* entry)
    {
        if (entry)
        {
            RemoveFromListIf(
                cont,
                [entry](const std::shared_ptr<T>& e)
                {
                    return (e.get() == entry);
                }
            );
        }
    }

    template <typename T, typename TBase>
    void RemoveFromUniqueSet(std::set<std::unique_ptr<T>>& cont, const TBase* entry)
    {
        if (entry)
        {
            RemoveFromListIf(
                cont,
                [entry](const std::unique_ptr<T>& e)
                {
                    return (e.get() == entry);
                }
            );
        }
    }

	template <typename T, typename TBase>
	void RemoveFromUniqueUnorderedMap(std::unordered_map<uuid, std::unique_ptr<T>>& cont, const TBase* entry)
	{
		if (entry)
		{
			RemoveFromListIf(
				cont,
				[entry](std::pair<const uuid, std::unique_ptr<T>>& e)
				{
					return (e.second.get() == entry);
				}
			);
		}
	}


    template <typename BaseType, typename SubType>
    SubType* TakeOwnership(std::set<std::unique_ptr<BaseType>>& objectSet, std::unique_ptr<SubType>&& object)
    {
        auto ref = object.get();
        objectSet.emplace(std::forward<std::unique_ptr<SubType>>(object));
        return ref;
    }

    template <typename BaseType, typename SubType>
    SubType* TakeOwnership(std::vector<std::unique_ptr<BaseType>>& objectSet, std::unique_ptr<SubType>&& object)
    {
        auto ref = object.get();
        objectSet.emplace_back(std::forward<std::unique_ptr<SubType>>(object));
        return ref;
    }

    template <typename BaseType, typename SubType>
    SubType* TakeOwnership(std::list<std::unique_ptr<BaseType>>& objectSet, std::unique_ptr<SubType>&& object)
    {
        auto ref = object.get();
        objectSet.emplace_back(std::forward<std::unique_ptr<SubType>>(object));
        return ref;
    }

	template <typename BaseType, typename SubType>
	SubType* TakeOwnership(std::unordered_map<uuid, std::unique_ptr<BaseType>>& objectSet, uuid uuid, std::unique_ptr<SubType>&& object)
	{
		auto ref = object.get();
		objectSet.insert(std::make_pair(uuid, std::forward<std::unique_ptr<SubType>>(object)));
		return ref;
	}

    template <typename ForwardIt, typename BinaryPredicate>
    static ForwardIt UniqueMerge(ForwardIt begin, ForwardIt end, BinaryPredicate pred)
    {
        if (begin == end)
            return end;

        ForwardIt result = begin;
        while (++begin != end)
        {
            if (!pred(*result, *begin) && ++result != begin)
                *result = std::move(*begin);
        }

        return ++result;
    }

    template <typename T>
    std::string ToHex(T value)
    {
        static_assert(std::is_integral<T>::value, "input parameter of 'LLGL::ToHex' must be an integral type");
        std::stringstream s;
        s << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << std::uppercase << static_cast<std::uint64_t>(value);
        return s.str();
    }
    
    /*template <typename TSub, typename TBase>
    TSub* NextArrayResource(std::uint32_t& numResources, TBase* const*& resourceArray)
    {
        if (numResources > 0)
        {
            --numResources;
            return LLGL_CAST(TSub*, (*(resourceArray++)));
        }
        return nullptr;
    }*/

    template <typename T>
    T GetAlignedSize(T size, T alignment)
    {
        if (alignment > 1)
        {
            const T alignmentBitmask = alignment - 1;
            return ((size + alignmentBitmask) & ~alignmentBitmask);
        }
        return size;
    }

	/*template <typename T>
	T Clamp(const T& x, const T& minimum, const T& maximum)
	{
		return std::max(minimum, std::min(x, maximum));
	}*/

    template <typename T>
    const void* GetMemberFuncPtr(T pfn)
    {
        union
        {
            T           func;
            const void* addr;
        }
        ptr;
        ptr.func = pfn;
        return ptr.addr;
    }

    template <typename T>
    inline std::size_t StrLength(const T* s)
    {
        std::size_t len = 0;
        while (*s++ != 0)
            ++len;
        return len;
    }

    template <>
    inline std::size_t StrLength<char>(const char* s)
    {
        return std::strlen(s);
    }

    template <>
    inline std::size_t StrLength<wchar_t>(const wchar_t* s)
    {
        return std::wcslen(s);
    }

    template <typename T>
    inline T* AdvancePtr(T* ptr, std::size_t offset)
    {
        using TByteAligned = typename std::conditional
            <
            std::is_const<T>::value,
            const std::int8_t,
            std::int8_t
            >::type;
        return reinterpret_cast<T*>(reinterpret_cast<TByteAligned*>(ptr) + offset);
    }

    GRAPHICS_DLL_DECLSPEC std::string ReadFileString(const char* filename);

    GRAPHICS_DLL_DECLSPEC std::vector<char> ReadFileBuffer(const char* filename);

    GRAPHICS_DLL_DECLSPEC std::string ToUTF8String(const std::wstring& utf16);
    GRAPHICS_DLL_DECLSPEC std::string ToUTF8String(const wchar_t* utf16);

    GRAPHICS_DLL_DECLSPEC std::wstring ToUTF16String(const std::string& utf8);
    GRAPHICS_DLL_DECLSPEC std::wstring ToUTF16String(const char* utf8);
}