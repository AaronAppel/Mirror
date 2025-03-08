#pragma once

// Example of user custom types declaration, or primitives unsupported by Mirror as default in TypeIds.h

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "MIR_Structs.h"

#include "main_Testing.h"
#include "MirrorTesting.h"

// Specialize TypeId to support additional types

#if defined(MIRROR_NONCONFORMING) && defined(MIRROR_GENERATE_TYPE_IDS)
MIRROR_TYPE_ID(float[10])

MIRROR_TYPE_ID(std::vector<char>)

MIRROR_TYPE_ID(std::pair<const int, bool>)
MIRROR_TYPE_ID(std::pair<std::string, int32_t>)
MIRROR_TYPE_ID(std::pair<const std::string, int32_t>)

MIRROR_TYPE_ID(std::map<int, bool>)
MIRROR_TYPE_ID(std::unordered_map<std::string, int32_t>)
// #TODO Problems in SetConstructionLambda() MIRROR_TYPE_ID(std::unordered_map<const std::string, int32_t>)

MIRROR_TYPE_ID(Derived1)
MIRROR_TYPE_ID(Derived1*)
MIRROR_TYPE_ID(Derived2)
MIRROR_TYPE_ID(Derived2*)
MIRROR_TYPE_ID(Base)
MIRROR_TYPE_ID(Base*)
MIRROR_TYPE_ID(TestStruct)
MIRROR_TYPE_ID(TestStruct*)

MIRROR_TYPE_ID(ExampleStruct)
MIRROR_TYPE_ID(ExampleClass)
MIRROR_TYPE_ID(ExampleDerivedClass)
MIRROR_TYPE_ID(ExampleNestedCustomTypes)
#else

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 1, float[10])

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 2, std::vector<char>)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 3, std::pair<const int, bool>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 4, std::pair<std::string, int32_t>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 5, std::pair<const std::string, int32_t>)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 6, std::map<int, bool>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 7, std::unordered_map<std::string, int32_t>)
// #TODO Problems in SetConstructionLambda() MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 8, std::unordered_map<const std::string, int32_t>)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 9, Derived1)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 10, Derived1*)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 11, Derived2)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 12, Derived2*)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 13, Base)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 14, Base*)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 15, TestStruct)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 16, TestStruct*)

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 17, ExampleStruct)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 18, ExampleClass)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 19, ExampleDerivedClass)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 20, ExampleNestedCustomTypes)

#endif