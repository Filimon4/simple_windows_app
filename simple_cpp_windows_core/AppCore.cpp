#include "pch.h"
#include "AppCore.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <type_traits>

static std::wstring lastResult;  // глобальный буфер, чтобы safely вернуть c_str()

template <typename T>
T convertToNumber(const std::wstring& str) {
    if constexpr (std::is_integral<T>::value)
        return static_cast<T>(std::stoi(str));
    else if constexpr (std::is_floating_point<T>::value)
        return static_cast<T>(std::stod(str));
    else
        throw std::invalid_argument("Unsupported type");
}

template <typename T>
std::vector<T> splitAndConvertToNumbers(const std::wstring& input) {
    std::vector<T> numbers;
    std::wistringstream stream(input);
    std::wstring token;
    while (std::getline(stream, token, L',')) {
        try {
            numbers.push_back(convertToNumber<T>(token));
        }
        catch (...) {}
    }
    return numbers;
}

template <typename T>
std::vector<T> swapAroundMaxIdx(std::vector<T> numbers) {
    if (numbers.empty()) return numbers;
    auto max_it = std::max_element(numbers.begin(), numbers.end());
    int max_idx = std::distance(numbers.begin(), max_it);
    std::vector<T> result(numbers.begin() + max_idx + 1, numbers.end());
    result.push_back(*max_it);
    result.insert(result.end(), numbers.begin(), numbers.begin() + max_idx);
    return result;
}

template <typename T>
std::wstring convertToWstring(const std::vector<T>& numbers) {
    std::wstring result;
    for (size_t i = 0; i < numbers.size(); ++i) {
        result += std::to_wstring(numbers[i]);
        if (i < numbers.size() - 1) result += L",";
    }
    return result;
}

std::vector<std::wstring> splitString(const std::wstring& str, const std::wstring& delimiter) {
    std::vector<std::wstring> result;
    size_t start = 0, end = str.find(delimiter);
    while (end != std::wstring::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}

template <typename T>
std::wstring processMatrix(const std::wstring& line) {
    auto matrixRows = splitString(line, L"\r\n");
    std::wstring result;
    for (size_t i = 0; i < matrixRows.size(); ++i) {
        if (matrixRows[i].empty()) continue;
        auto numbers = splitAndConvertToNumbers<T>(matrixRows[i]);
        auto swapped = swapAroundMaxIdx(numbers);
        result += convertToWstring(swapped);
        if (i < matrixRows.size() - 1) result += L"\r\n";
    }
    return result;
}

template <typename T>
std::wstring processArray(const std::wstring& line) {
    auto numbers = splitAndConvertToNumbers<T>(line);
    auto swapped = swapAroundMaxIdx(numbers);
    return convertToWstring(swapped);
}

const wchar_t* processArrayInt(const wchar_t* input) {
    lastResult = processArray<int>(input);
    return lastResult.c_str();
}

const wchar_t* processMatrixInt(const wchar_t* input) {
    lastResult = processMatrix<int>(input);
    return lastResult.c_str();
}
