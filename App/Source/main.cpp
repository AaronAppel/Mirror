
#ifdef MIRROR_EXAMPLES
#include "main.h"

#include <cassert>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "../../cJSON/Source/cJSON.h"

#include "Mirror.h"

#include "Serialization/Serialize.h"

#include "ConstexprCounter.h"

const char* const g_filePath = "Mirror.json";

MIRROR_CLASS_START(ExampleStruct)
MIRROR_CLASS_MEMBER(intA);
MIRROR_CLASS_MEMBER(boolB);
MIRROR_CLASS_MEMBER(charC);
MIRROR_CLASS_MEMBER(floatD);
MIRROR_CLASS_MEMBER(doubleE);
MIRROR_CLASS_MEMBER(constCharPtrF);
MIRROR_CLASS_MEMBER(stdStringG);
MIRROR_CLASS_MEMBER(exampleMapH);
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
	auto resultExperimental = Mirror::TypeId<float>();
	resultExperimental = Mirror::TypeId<int>();
	resultExperimental = Mirror::TypeId<ExampleClass>();

	ExampleClass thing;
	Serialize::ToFile(thing, "file.txt");
	Serialize::FromFile("file.txt", thing);

	resultExperimental = 1;
	resultExperimental = Mirror::TypeId<void>();

	switch (resultExperimental)
	{
	case Mirror::TypeId<std::string>():
	case Mirror::TypeId<char*>():
	case Mirror::TypeId<const char*>():
	 	break;

	case Mirror::TypeId<void>():
		{
			int bp = 0;
		}
		break;

	case Mirror::TypeId<void*>():
		{
			int bp = 0;
		}
		break;

	case Mirror::TypeId<float>():
		{
			int bp = 0;
		}
		break;

	case Mirror::TypeId<int>():
		{
			int bp = 0;
		}
		break;

	case Mirror::TypeId<ExampleClass>():
		{
			int bp = 0;
		}
		break;

	default:
		break;
	}

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

	auto id = Mirror::TypeId<ExampleClass>();
}

#endif
