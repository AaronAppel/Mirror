#pragma once

// Example of user custom types declaration, or primitives unsupported by Mirror as default in TypeIds.h

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "Structs.h"

// Specialize TypeId to support additional types
MIRROR_TYPE_ID(float[10])

MIRROR_TYPE_ID(std::vector<char>)

MIRROR_TYPE_ID(std::pair<const int, bool>)
MIRROR_TYPE_ID(std::pair<std::string, int32_t>)
MIRROR_TYPE_ID(std::pair<const std::string, int32_t>)

MIRROR_TYPE_ID(std::map<int, bool>)
MIRROR_TYPE_ID(std::unordered_map<std::string, int32_t>)
MIRROR_TYPE_ID(std::unordered_map<const std::string, int32_t>)
