#pragma once

#include <map>
#include <string>
#include <vector>

#include "MIR_Structs.h"

#include "main.h"

// Declare custom types to generate type ids

#if defined(MIRROR_NONCONFORMING) && defined(MIRROR_GENERATE_TYPE_IDS)

MIRROR_TYPE_ID(float[10])
MIRROR_TYPE_ID(std::vector<char>)
MIRROR_TYPE_ID(std::pair<const int, bool>)
MIRROR_TYPE_ID(std::map<int, bool>)
MIRROR_TYPE_ID(ExampleClass)
MIRROR_TYPE_ID(ExampleDerivedClass)
MIRROR_TYPE_ID(ExampleNestedCustomTypes)

#else

MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 1,	float[10])
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 2,	std::vector<char>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 3,	std::pair<const int, bool>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 4,	std::map<int, bool>)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 5,	ExampleClass)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 6,	ExampleDerivedClass)
MIRROR_TYPE_ID(MIRROR_USER_TYPE_ID_START + 7,	ExampleNestedCustomTypes)

#endif
