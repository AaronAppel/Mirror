#pragma once

#include "Structs.h"

// Specialize TypeIdConstexpr to support additional types
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::pair<const int, bool>>() { return 59; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::map<int, bool>>() { return 60; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<float[10]>() { return 61; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::vector<char>>() { return 62; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::pair<std::string, int32_t>>() { return 63; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::unordered_map<std::string, int32_t>>() { return 64; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::pair<const std::string, int32_t>>() { return 65; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::unordered_map<const std::string, int32_t>>() { return 66; }
