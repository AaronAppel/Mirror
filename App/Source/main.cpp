
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

MIRROR_CLASS_START(ExampleStruct)
MIRROR_CLASS_MEMBER(intA);
MIRROR_CLASS_MEMBER(boolB);
MIRROR_CLASS_MEMBER(charC);
MIRROR_CLASS_MEMBER(floatD);
MIRROR_CLASS_MEMBER(doubleE);
MIRROR_CLASS_MEMBER(constCharPtrF);
MIRROR_CLASS_MEMBER(stdStringG);
MIRROR_CLASS_MEMBER(exampleMapH);
MIRROR_CLASS_END

MIRROR_CLASS_START(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intZ)
MIRROR_CLASS_END

MIRROR_CLASS_START(ExampleClass)
MIRROR_CLASS_SUBCLASS(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intX)
MIRROR_CLASS_MEMBER(intY)
MIRROR_CLASS_END

MIRROR_CLASS_START(ExampleNestedCutomTypes)
MIRROR_CLASS_MEMBER(exStruct)
MIRROR_CLASS_MEMBER(exClass)
MIRROR_CLASS_END

int main()
{
	auto resultExperimental = Mirror::TypeIdConstexpr<float>();
	resultExperimental = Mirror::TypeIdConstexpr<int>();
	resultExperimental = Mirror::TypeIdConstexpr<ExampleClass>();

	ExampleClass thing;
	Serialize::ToFile(thing, "file.txt");
	Serialize::FromFile("file.txt", thing);

	switch (resultExperimental)
	{
	case Mirror::TypeIdConstexpr<float>():
		{
			int bp = 0;
		}
		break;

	case Mirror::TypeIdConstexpr<int>():
		{
			int bp = 0;
		}
		break;

	case Mirror::TypeIdConstexpr<ExampleClass>():
		{
			int bp = 0;
		}
		break;

	default:
		break;
	}

	// Func();
	auto result = Mirror::TypeIdConstexpr<float>();
	const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<float>();

	result = Mirror::TypeIdConstexpr<int64_t>();
	typeInfo = Mirror::InfoForType<int64_t>();

	result = Mirror::TypeIdConstexpr<long long>();
	typeInfo = Mirror::InfoForType<long long>();

	result = Mirror::TypeIdConstexpr<bool>();
	typeInfo = Mirror::InfoForType<bool>();

	result = Mirror::TypeIdConstexpr<int>();
	typeInfo = Mirror::InfoForType<int>();

	result = Mirror::TypeIdConstexpr<std::string>();
	typeInfo = Mirror::InfoForType<std::string>();

	const ExampleStruct comparison;
	ExampleStruct a;
	Serialize::ToFile(a, "file.txt");

	assert(comparison.floatD == a.floatD && "Mismatched floats!");

	// #TODO Debug this line
	// Serialize::FromFile("file.txt", a);

	auto id = Mirror::TypeIdConstexpr<ExampleClass>();
}

#endif
