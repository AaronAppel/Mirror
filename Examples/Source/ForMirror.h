#pragma once

#include <array>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "../../Source/MIR_Mirror.h"

#include "Main.h"

// #MIRROR:
// Here we declare types we want Mir to reflect.
// Both MIRROR_TYPE_ID() and MIRROR_TYPE/MIRROR_CLASS should be added for each declared type.
// MIRROR_TYPE_ID has no order dependency.
// Categories can help with organization.

// Serialized types
MIRROR_TYPE_ID(1, std::string[10])

// Non-serialized types
MIRROR_TYPE_ID(100, float[10]) // #NOTE Reserving 99 IDs for future serialized types
MIRROR_TYPE_ID(101, std::vector<char>)
MIRROR_TYPE_ID(102, std::pair<const int, bool>)
MIRROR_TYPE_ID(103, std::map<int, bool>)
MIRROR_TYPE_ID(104, ExampleChildClass)
MIRROR_TYPE_ID(105, ExampleClass)
MIRROR_TYPE_ID(106, ExampleParentClass)
MIRROR_TYPE_ID(107, ExampleNestedCustomTypes)
MIRROR_TYPE_ID(108, std::array<int, 10>)
MIRROR_TYPE_ID(109, UserCollection)
MIRROR_TYPE_ID(110, std::filesystem::path)
MIRROR_TYPE_ID(111, int[10])