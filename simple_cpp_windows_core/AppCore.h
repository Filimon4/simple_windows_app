#pragma once
#ifdef APPCORE_EXPORTS
#define APPCORE_API __declspec(dllexport)
#else
#define APPCORE_API __declspec(dllimport)
#endif
#include <string>


extern "C" __declspec(dllexport) std::wstring processArrayInt(const std::wstring& input);
extern "C" __declspec(dllexport) std::wstring processMatrixInt(const std::wstring& input);