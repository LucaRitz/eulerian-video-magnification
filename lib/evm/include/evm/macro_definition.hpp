#pragma once
#if defined(_MSC_VER)
// Windows
    #if defined(EVM_IMPORT)
        #define EXPORT_EVM __declspec(dllimport)
    #else
        #define EXPORT_EVM __declspec(dllexport)
    #endif
#elif defined(__GNUC__)
//  GCC
    #define EXPORT_EVM __attribute__((visibility("default")))
#else
    //  do nothing and hope for the best?
    #define EXPORT_EVM
    #pragma warning Unknown dynamic link import/export semantics.
#endif