
#ifdef MIRROR_EXAMPLES
#include "main.h"

#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "../../cJSON/Source/cJSON.h"

#include "Mirror.h"

#include "Serialization/Serialize.h"

const char* const g_filePath = "Mirror.json";

bool FileExists();

MIRROR_CLASS_START(ExampleStruct)
MIRROR_CLASS_MEMBER(intA);
MIRROR_CLASS_MEMBER(boolB);
MIRROR_CLASS_MEMBER(charC);
MIRROR_CLASS_MEMBER(floatD);
MIRROR_CLASS_MEMBER(doubleE);
MIRROR_CLASS_MEMBER(constCharPtrF);
MIRROR_CLASS_MEMBER(stdStringG);
// #TODO MIRROR_CLASS_MEMBER(exampleMapH);
MIRROR_CLASS_END(ExampleStruct)

MIRROR_CLASS_START(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intZ)
MIRROR_CLASS_END(ExampleDerivedClass)

MIRROR_CLASS_START(ExampleClass)
MIRROR_CLASS_SUBCLASS(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intX)
MIRROR_CLASS_MEMBER(intY)
MIRROR_CLASS_END(ExampleClass)

MIRROR_CLASS_START(ExampleNestedCutomTypes)
MIRROR_CLASS_MEMBER(exStruct)
MIRROR_CLASS_MEMBER(exClass)
MIRROR_CLASS_END(ExampleNestedCutomTypes)

int main()
{
	// Func();
	auto result = Mirror::TypeId<float>();
	const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<float>();

	result = Mirror::TypeId<int64_t>();
	typeInfo = Mirror::InfoForType<int64_t>();

	result = Mirror::TypeId<long long>();
	typeInfo = Mirror::InfoForType<long long>();

	result = Mirror::TypeId<bool>();
	typeInfo = Mirror::InfoForType<bool>();

	result = Mirror::TypeId<int>();
	typeInfo = Mirror::InfoForType<int>();

	result = Mirror::TypeId<std::string>();
	typeInfo = Mirror::InfoForType<std::string>();

	const ExampleStruct comparison;
	ExampleStruct a;
	Serialize::ToFile(a, "file.txt");

	assert(comparison.floatD == a.floatD && "Mismatched floats!");

	Serialize::FromFile("file.txt", a);
	// #TODO Have a switch statement to check uniqueness of all generated type ids

	auto id = Mirror::TypeId<ExampleClass>();

	// template <typename T>
	// using TypeCase<T> = Mirror::TypeId<T>();
	// case TypeCase<uint8_t>: break;

	switch (typeInfo->id)
	{
	case Mirror::TypeId<uint8_t>():
		break;

	case Mirror::TypeId<ExampleClass>():
		break;

	case Mirror::TypeId<int32_t>():
		break;

	case Mirror::TypeId<uint16_t>():
		break;

	case Mirror::TypeId<uint32_t>():
		break;

	default:
		break;
	}
}

#endif
