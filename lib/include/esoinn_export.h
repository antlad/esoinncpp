
#ifndef _ESOINN_DLLDEFINES_H_
#define _ESOINN_DLLDEFINES_H_

#if _WIN32

#if ESOINN_EXPORT_DLL
#define ESOINN_EXPORT __declspec(dllexport)
#else
#define ESOINN_EXPORT __declspec(dllimport)
#endif

#define STDCALL __stdcall
#else /* defined (_WIN32) */
#define ESOINN_EXPORT
#endif

#endif /* _ESOINN_DLLDEFINES_H_ */
