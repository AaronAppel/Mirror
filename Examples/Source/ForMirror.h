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
// Both MIR_TYPE_ID() and MIR_TYPE/MIR_CLASS should be added for each declared type.
// MIR_TYPE_ID has no order dependency.
// Categories can help with organization.

// Serialized types
MIR_TYPE_ID(1, std::string[10])

// Non-serialized types
MIR_TYPE_ID(100, float[10]) // #NOTE Reserving 99 IDs for future serialized types
MIR_TYPE_ID(101, std::vector<char>)
MIR_TYPE_ID(102, std::pair<const int, bool>)
MIR_TYPE_ID(103, std::map<int, bool>)
MIR_TYPE_ID(104, ExampleChildClass)
MIR_TYPE_ID(105, ExampleClass)
MIR_TYPE_ID(106, ExampleParentClass)
MIR_TYPE_ID(107, ExampleNestedCustomTypes)
MIR_TYPE_ID(108, std::array<int, 10>)
MIR_TYPE_ID(109, UserCollection)
MIR_TYPE_ID(110, std::filesystem::path)
MIR_TYPE_ID(111, int[10])

MIR_TYPE_ID(112, std::tuple<int, char, bool, int, char, bool>)
MIR_TYPE_ID(113, std::tuple<int>)
MIR_TYPE_ID(114, std::tuple<int, bool, char, double, float>);
