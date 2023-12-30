#pragma once

/** MSVC */
#define WTHREAD_IMPORT __declspec(dllimport)
#define WTHREAD_EXPORT __declspec(dllexport)

#ifdef WTHREAD_BUILD_DLL

#ifdef WTHREAD_DLL
#define WTHREAD_API WTHREAD_EXPORT
#else // WTHREAD_DLL
#define WTHREAD_API WTHREAD_IMPORT
#endif // WTHREAD_DLL

#else // WTHREAD_BUILD_DLL
#define WTHREAD_API
#endif // WTHREAD_BUILD_DLL

#define WTHREAD_FUNC std::function<int(void)>
