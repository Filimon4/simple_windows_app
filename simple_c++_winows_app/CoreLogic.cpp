//#include "CoreLogic.h"
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <vector>
//#include <type_traits> 
//#include <cstddef>
//
//template <typename T>
//T convertToNumber(const std::wstring& str) {
//    if constexpr (std::is_integral<T>::value) {
//        return static_cast<T>(std::stoi(str));
//    }
//    else if constexpr (std::is_floating_point<T>::value) {
//        return static_cast<T>(std::stod(str));
//    }
//    else if constexpr (std::is_same<T, std::byte>::value) {
//        return static_cast<std::byte>(std::stoi(str));
//    }
//    else {
//        throw std::invalid_argument("Unsupported type");
//    }
//}
//
//template <typename T>
//std::vector<T> swapAroundMaxIdx(std::vector<T> numbers) {
//    if (numbers.empty()) return numbers;
//
//    auto max_it = std::max_element(numbers.begin(), numbers.end());
//    int max_idx = std::distance(numbers.begin(), max_it);
//    std::vector<T> result;
//
//    for (int i = max_idx + 1; i < numbers.size(); ++i)
//        result.push_back(numbers[i]);
//
//    result.push_back(numbers[max_idx]);
//
//    for (int i = 0; i < max_idx; ++i)
//        result.push_back(numbers[i]);
//
//    return result;
//}
//
//template <typename T>
//std::vector<T> splitAndConvertToNumbers(const std::wstring& input) {
//    std::vector<T> numbers;
//    std::wistringstream stream(input);
//    std::wstring token;
//
//    while (std::getline(stream, token, L',')) {
//        try {
//            numbers.push_back(convertToNumber<T>(token));
//        }
//        catch (...) {
//        }
//    }
//    return numbers;
//}
//
//
//template <typename T>
//std::wstring convertToWstring(const std::vector<T>& numbers) {
//    std::wstring result;
//    for (size_t i = 0; i < numbers.size(); ++i) {
//        result += std::to_wstring(numbers[i]);
//        if (i < numbers.size() - 1) {
//            result += L",";
//        }
//    }
//    return result;
//}
//
//template <>
//std::wstring convertToWstring<std::byte>(const std::vector<std::byte>& numbers) {
//    std::wstring result;
//    for (size_t i = 0; i < numbers.size(); ++i) {
//        result += std::to_wstring(static_cast<unsigned>(numbers[i]));
//        if (i < numbers.size() - 1) {
//            result += L",";
//        }
//    }
//    return result;
//}
//
//template <typename T>
//std::wstring processArray(const std::wstring& line) {
//    std::vector<T> numbers = splitAndConvertToNumbers<T>(line);
//    std::vector<T> swapNumbers = swapAroundMaxIdx<T>(numbers);
//    return convertToWstring<T>(swapNumbers);
//}
//
//std::vector<std::wstring> splitString(const std::wstring& str, const std::wstring& delimiter) {
//    std::vector<std::wstring> result;
//    size_t start = 0;
//    size_t end = str.find(delimiter);
//
//    while (end != std::wstring::npos) {
//        result.push_back(str.substr(start, end - start));
//        start = end + delimiter.length();
//        end = str.find(delimiter, start);
//    }
//
//    result.push_back(str.substr(start));
//    return result;
//}
//
//template <typename T>
//std::wstring processMatrix(const std::wstring& line) {
//    std::vector<std::wstring> matrixRows = splitString(line, L"\r\n");
//
//    std::wstring result;
//
//    for (auto& row : matrixRows) {
//        if (row.empty()) continue;
//
//        std::vector<T> numbers = splitAndConvertToNumbers<T>(row);
//        std::vector<T> swappedNumbers = swapAroundMaxIdx<T>(numbers);
//
//        result += convertToWstring<T>(swappedNumbers);
//
//        if (&row != &matrixRows.back()) {
//            result += L"\r\n";
//        }
//    }
//
//    return result;
//}
//
//template std::wstring processArray<int>(const std::wstring& line);
//template std::wstring processArray<std::byte>(const std::wstring& line);
//template std::wstring processMatrix<int>(const std::wstring& line);
//template std::wstring processMatrix<std::byte>(const std::wstring& line);
