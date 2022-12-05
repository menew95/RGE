#pragma once

#include "Common/Types.h"

#include <Windows.h>
#include <vector>

class StringHelper
{
public:
    /**
     * @brief 문자열을 포매팅합니다.
     * @param _format 포매팅할 문자열 형식입니다.
     * @param ..._args (optional) 포매팅할 문자열에 들어갈 값들입니다.
     * @return
    */
    template <typename ... Args>
    static tstring Format(const tstring& _format, Args ... _args);

    /**
     * @brief 문자열을 포매팅합니다.
     * @param _format 포매팅할 문자열 형식입니다.
     * @param ..._args (optional) 포매팅할 문자열에 들어갈 값들입니다.
     * @return 포매팅된 문자열입니다.
    */
    template <typename ... Args>
    static tstring Format(const TCHAR* _format, Args ... _args);

    /**
     * @brief 문자열을 주어진 토큰 기준으로 나눕니다.
     * @param _str 토큰으로 나눌 문자열입니다.
     * @param _token 나눌 구분자 토큰입니다.
     * @return 토큰으로 나누어진 문자열 배열입니다.
    */
    static std::vector<tstring> Split(const tstring& _str, const tstring& _token);

    /**
     * @brief string을 wstring으로 변환합니다.
    */
    static std::wstring StringToWString(const char* _string);

    /**
     * @brief string을 wstring으로 변환합니다.
    */
    static std::wstring StringToWString(const std::string& _string);

    /**
     * @brief wstring을 string으로 변환합니다.
    */
    static std::string WStringToString(const wchar_t* _string);

    /**
     * @brief wstring을 string으로 변환합니다.
    */
    static std::string WStringToString(const std::wstring& _string);

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 wstring으로 변환합니다.
    */
    static std::wstring ToWString(const std::string& _string)
    {
        return StringToWString(_string);
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 wstring으로 변환합니다.
    */
    static std::wstring ToWString(const std::wstring& _string)
    {
        return _string;
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 wstring으로 변환합니다.
    */
    static std::wstring ToWString(const char* _string)
    {
        return StringToWString(_string);
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 wstring으로 변환합니다.
    */
    static std::wstring ToWString(const wchar_t* _string)
    {
        return std::wstring(_string);
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 string으로 변환합니다.
    */
    static std::string ToString(const std::wstring& _string)
    {
        return WStringToString(_string);
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 string으로 변환합니다.
    */
    static std::string ToString(const std::string& _string)
    {
        return _string;
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 string으로 변환합니다.
    */
    static std::string ToString(const wchar_t* _string)
    {
        return WStringToString(_string);
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 string으로 변환합니다.
    */
    static std::string ToString(const char* _string)
    {
        return std::string{ _string };
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 tstring으로 변환합니다.
    */
    static tstring ToTString(const std::string& _string)
    {
#if defined (_UNICODE)
        return StringToWString(_string);
#else
        return _string;
#endif
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 tstring으로 변환합니다.
    */
    static tstring ToTString(const std::wstring& _string)
    {
#if defined (_UNICODE)
        return _string;
#else
        return WStringToString(_string);
#endif
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 tstring으로 변환합니다.
    */
    static tstring ToTString(const char* _string)
    {
#if defined( _UNICODE )
        return StringToWString(_string);
#else
        return std::string{ _string };
#endif
    }

    /**
     * @brief 어떤 문자열 타입이든 상관 없이 tstring으로 변환합니다.
    */
    static tstring ToTString(const wchar_t* _string)
    {
#if defined( _UNICODE )
        return std::wstring{ _string };
#else
        return WStringToString(_string);
#endif
    }
};

#include "StringHelper.inl"
