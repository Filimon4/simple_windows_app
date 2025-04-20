#pragma once
#ifdef APPCORE_EXPORTS
#define APPCORE_API __declspec(dllexport)
#else
#define APPCORE_API __declspec(dllimport)
#endif
#include <string>


extern "C" APPCORE_API const wchar_t* processArrayInt(const wchar_t* input);
extern "C" APPCORE_API const wchar_t* processArrayByte(const wchar_t* input);
extern "C" APPCORE_API const wchar_t* processMatrixInt(const wchar_t* input);
extern "C" APPCORE_API const wchar_t* processMatrixByte(const wchar_t* input);