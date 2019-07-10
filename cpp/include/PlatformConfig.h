#pragma once

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported target platform macro which CC uses.
#define PLATFORM_UNKNOWN            0
#define PLATFORM_IOS                1
#define PLATFORM_ANDROID            2
#define PLATFORM_WIN32              3
#define PLATFORM_MARMALADE          4
#define PLATFORM_LINUX              5
#define PLATFORM_BADA               6
#define PLATFORM_BLACKBERRY         7
#define PLATFORM_MAC                8
#define PLATFORM_NACL               9
#define PLATFORM_EMSCRIPTEN        10
#define PLATFORM_TIZEN             11
#define PLATFORM_QT5               12
#define PLATFORM_WINRT             13

// Determine target platform by compile environment macro.
#define TARGET_PLATFORM             PLATFORM_WIN32

// Apple: Mac and iOS
#if defined(__APPLE__) && !defined(ANDROID) // exclude android for binding generator.
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
        #undef  TARGET_PLATFORM
        #define TARGET_PLATFORM         PLATFORM_IOS
    #elif TARGET_OS_MAC
        #undef  TARGET_PLATFORM
        #define TARGET_PLATFORM         PLATFORM_MAC
    #endif
#endif

// android
#if defined(ANDROID)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM         PLATFORM_ANDROID
#endif

// win32
#if defined(_WIN32) && defined(_WINDOWS)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM         PLATFORM_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM         PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  TARGET_PLATFORM
#define TARGET_PLATFORM         PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  TARGET_PLATFORM
#define TARGET_PLATFORM         PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM     PLATFORM_BLACKBERRY
#endif

// native client
#if defined(__native_client__)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM     PLATFORM_NACL
#endif

// Emscripten
#if defined(EMSCRIPTEN)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM     PLATFORM_EMSCRIPTEN
#endif

// tizen
#if defined(TIZEN)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM     PLATFORM_TIZEN
#endif

// qt5
#if defined(TARGET_QT5)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM     PLATFORM_QT5
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
    #undef  TARGET_PLATFORM
    #define TARGET_PLATFORM          PLATFORM_WINRT
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif

#if (TARGET_PLATFORM == PLATFORM_WIN32)
#ifndef __MINGW32__
#pragma warning (disable:4127)
#endif
#endif  // PLATFORM_WIN32

#if ((TARGET_PLATFORM == PLATFORM_ANDROID) || (TARGET_PLATFORM == PLATFORM_IOS) || (TARGET_PLATFORM == PLATFORM_TIZEN))
    #define PLATFORM_MOBILE
#else
    #define PLATFORM_PC
#endif


