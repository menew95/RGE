/**

    @file      Singleton.h
    @brief     싱글턴 패턴 매크로
    @author    LWT
    @date      3.07.2023

**/
#pragma once

#define SINGTON(Class) public:\
Class* Get();
#define SINGTON_IMPL(Class, ...)  std::shared_ptr<Class> g_Instance = nullptr; \
Class* Class::Get() \
{\
    if (g_Instance == nullptr)\
    {\
        g_Instance = std::make_shared<Class>(##__VA_ARGS__);\
    }\
    return g_Instance.get();\
}