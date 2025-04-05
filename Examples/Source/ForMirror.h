#pragma once

#include <map>
#include <string>
#include <vector>

#include "MIR_Structs.h"

#include "Main.h"

// #TODO Review comment
// MIRROR:
// Here we declare types we want Mirror to reflect.
// Both MIRROR_TYPE_ID() and MIRROR_TYPE/MIRROR_CLASS should be added for each type.

// Serialized (order dependent) Order is not important, but it can help with organization
MIRROR_TYPE_ID(1, std::string[10])

// Non-serialized
MIRROR_TYPE_ID(100, float[10]) // #NOTE Leaving a 100 spaced buffer for future serialized types
MIRROR_TYPE_ID(101, std::vector<char>)
MIRROR_TYPE_ID(102, std::pair<const int, bool>)
MIRROR_TYPE_ID(103, std::map<int, bool>)
MIRROR_TYPE_ID(104, ExampleClass)
MIRROR_TYPE_ID(105, ExampleDerivedClass)
MIRROR_TYPE_ID(106, ExampleNestedCustomTypes)
