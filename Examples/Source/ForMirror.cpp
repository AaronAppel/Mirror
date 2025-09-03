#include "MIR_Mirror.h"

#include "Main.h"
#include "ForMirror.h"

// #MIRROR:
// Here we declare types we want Mir to reflect.
// Both MIR_TYPE_ID() and MIR_TYPE/MIR_CLASS should be added for each declared type.
// MIR_TYPE has no order dependency.
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
MIR_TYPE(std::string[10])
MIR_TYPE(std::filesystem::path)

// Non-serialized types
MIR_TYPE(float[10])
MIR_TYPE(std::array<int, 10>)
MIR_TYPE(std::vector<char>)
MIR_TYPE(std::pair<const int, bool>)
MIR_TYPE(std::map<int, bool>)

MIR_CLASS(ExampleChildClass)
MIR_CLASS_MEMBER_FLAGS(intZ, 1 << 0)
MIR_CLASS_END

MIR_CLASS(ExampleClass)
MIR_CLASS_MEMBER_FLAGS(intA, ExampleFlags::Flag2 | ExampleFlags::Flag3)
MIR_CLASS_MEMBER(boolB)
MIR_CLASS_MEMBER(charC)
MIR_CLASS_MEMBER(floatD)
MIR_CLASS_MEMBER(doubleE)
MIR_CLASS_MEMBER(constCharPtrF)
MIR_CLASS_MEMBER(stdStringG)
MIR_CLASS_MEMBER(exampleMapH)
MIR_CLASS_END

MIR_CLASS(ExampleParentClass)
MIR_CLASS_END

MIR_CLASS(ExampleNestedCustomTypes)
MIR_CLASS_MEMBER(exClass)
MIR_CLASS_END

// #TODO Revisit inheritance relational reflection
// MIRROR_PARENT_CHILD(ExampleParentClass, ExampleClass)
// MIRROR_PARENT_CHILD(ExampleClass, ExampleChildClass)
