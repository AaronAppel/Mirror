#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Structs.h"

// Specialize TypeId to support additional types

// #NOTE Hard coding/manually defining instead of using something like __COUNTER__ to avoid id collisions with classes
#define DECLARE_TYPE_ID(TYPE, ID) \
template <> constexpr std::size_t Mirror::TypeId<TYPE>() { return ID; }

template <> constexpr std::size_t Mirror::TypeId<std::pair<const int, bool>>() { return 59; }
template <> constexpr std::size_t Mirror::TypeId<std::map<int, bool>>() { return 60; }
DECLARE_TYPE_ID(float[10], 61)
DECLARE_TYPE_ID(std::vector<char>, 62)
template <> constexpr std::size_t Mirror::TypeId<std::pair<std::string, int32_t>>() { return 63; }
template <> constexpr std::size_t Mirror::TypeId<std::unordered_map<std::string, int32_t>>() { return 64; }
template <> constexpr std::size_t Mirror::TypeId<std::pair<const std::string, int32_t>>() { return 65; }
template <> constexpr std::size_t Mirror::TypeId<std::unordered_map<const std::string, int32_t>>() { return 66; }
