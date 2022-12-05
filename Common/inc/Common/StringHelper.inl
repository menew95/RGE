#pragma once

#include <stringapiset.h>

template <typename ... Args>
tstring StringHelper::Format(const tstring& _format, Args ... _args)
{
    return ToTString(_format.c_str(), _args ...);
}

template <typename ... Args>
tstring StringHelper::Format(const TCHAR* _format, Args ... _args)
{
    static constexpr int BUFFER_SIZE = 1024;

    TCHAR _buffer[BUFFER_SIZE];
    _sntprintf_s(_buffer, BUFFER_SIZE, _format, _args ...);

    const size_t _length = _tcslen(_buffer);
    return tstring{ _buffer, _buffer + _length };
}

inline std::vector<tstring> StringHelper::Split(const tstring& _str, const tstring& _token)
{
    // Check parameters
    if (_token.length() == 0 || _str.find(_token) == tstring::npos)
        return std::vector<tstring>({ _str });


    // return var
    std::vector<tstring> _retList;


    size_t _findOffset = 0;
    size_t _splitOffset = 0;
    while ((_splitOffset = _str.find(_token, _findOffset)) != tstring::npos)
    {
        _retList.push_back(_str.substr(_findOffset, _splitOffset - _findOffset));
        _findOffset = _splitOffset + _token.length();
    }
    _retList.push_back(_str.substr(_findOffset, _str.length() - _findOffset));

    return _retList;
}

inline std::wstring StringHelper::StringToWString(const char* _string)
{
    const int _stringLength = static_cast<int>(std::strlen(_string));
    const int _bufferLength = _stringLength + 1;
    wchar_t* _buffer = new wchar_t[_bufferLength];

    // MultiByteToWideChar 함수가 문자열의 끝에 자동으로 null 문자 ('\0')을 넣어주지 않습니다.
    // 따라서 문자열을 변환을 마친 후 그 뒤에다 수동으로 null문자를 넣어주어야 합니다.
    int _end = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, _string, _stringLength,
        _buffer, _bufferLength);
    _buffer[_end] = '\0';

    auto _retVal = std::wstring{ _buffer };

    delete[] _buffer;
    return _retVal;
}

inline std::wstring StringHelper::StringToWString(const std::string& _string)
{
    return StringToWString(_string.c_str());
}

inline std::string StringHelper::WStringToString(const wchar_t* _string)
{
    const int _stringLength = static_cast<int>(std::wcslen(_string));
    const int _bufferLength = _stringLength * 2 + 1;
    char* _buffer = new char[_bufferLength];

    // WideCharToMultiByte 함수가 문자열의 끝에 자동으로 null 문자 ('\0')을 넣어주지 않습니다.
    // 따라서 문자열을 변환을 마친 후 그 뒤에다 수동으로 null문자를 넣어주어야 합니다.
    int _end = WideCharToMultiByte(CP_UTF8, 0, _string, _stringLength,
        _buffer, _bufferLength,
        nullptr, nullptr);
    _buffer[_end] = '\0';

    auto _retVal = std::string{ _buffer };

    delete[] _buffer;
    return _retVal;
}

inline std::string StringHelper::WStringToString(const std::wstring& _string)
{
    return WStringToString(_string.c_str());
}
