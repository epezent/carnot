#ifndef SFVG_CONFIG_HPP
#define SFVG_CONFIG_HPP

// Define DLL import/export macros (only Windows, and only dynamic configuration)
#if (defined(_WIN32) || defined(__WIN32__)) && !defined(SFML_STATIC)

    // Export dynamic link library (from DLL side)
    #ifdef SFVG_EXPORTS
        #define SFVG_API __declspec(dllexport)

    // Import dynamic link library (from client side)
    #else
        #define SFVG_API __declspec(dllimport)

    #endif // SFVG_EXPORTS

    // Disable annoying MSVC++ warning
    #ifdef _MSC_VER
        #pragma warning(disable: 4251)
    #endif // _MSC_VER

// Other platforms don't have DLLs
#else
    #define SFVG_API
#endif

// Version of the library
#define SFVG_VERSION_MAJOR 0
#define SFVG_VERSION_MINOR 1

#endif // SFVG_CONFIG_HPP
