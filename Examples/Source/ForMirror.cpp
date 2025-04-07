#include <map>
#include <string>
#include <vector>

#include "MIR_Mirror.h"

#include "Main.h"
#include "ForMirror.h"

// #MIRROR:
// Here we declare types we want Mirror to reflect.
// Both MIRROR_TYPE_ID() and MIRROR_TYPE/MIRROR_CLASS should be added for each declared type.
// MIRROR_TYPE has no order dependency.
// Categories can help with organization.

enum ExampleFlags : uint8_t
{
	Flag1 = 1 << 0,
	Flag2 = 1 << 1,
	Flag3 = 1 << 2,
	Flag4 = 1 << 3,
	Flag5 = 1 << 4,
	Flag6 = 1 << 5,
	Flag7 = 1 << 6,
	Flag8 = 1 << 7
};

// Serialized types (.json)
MIRROR_TYPE(std::string[10])

// Non-serialized types
MIRROR_TYPE(float[10])
MIRROR_TYPE(std::vector<char>)
MIRROR_TYPE(std::pair<const int, bool>)
MIRROR_TYPE(std::map<int, bool>)

MIRROR_CLASS(ExampleChildClass)
MIRROR_CLASS_MEMBER_FLAGS(intZ, 1 << 0)
MIRROR_CLASS_END

MIRROR_CLASS(ExampleClass)
MIRROR_CLASS_MEMBER_FLAGS(intA, ExampleFlags::Flag2 | ExampleFlags::Flag3)
MIRROR_CLASS_MEMBER(boolB)
MIRROR_CLASS_MEMBER(charC)
MIRROR_CLASS_MEMBER(floatD)
MIRROR_CLASS_MEMBER(doubleE)
MIRROR_CLASS_MEMBER(constCharPtrF)
MIRROR_CLASS_MEMBER(stdStringG)
MIRROR_CLASS_MEMBER(exampleMapH)
MIRROR_CLASS_END

MIRROR_CLASS(ExampleParentClass)
MIRROR_CLASS_END

MIRROR_CLASS(ExampleNestedCustomTypes)
MIRROR_CLASS_MEMBER(exClass)
MIRROR_CLASS_END

MIRROR_PARENT_CHILD(ExampleParentClass, ExampleClass)
MIRROR_PARENT_CHILD(ExampleClass, ExampleChildClass)
