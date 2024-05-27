
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
	auto stringId = MIRROR_TYPE_ID(std::string);
	auto stringId2 = MIRROR_TYPE_ID(decltype(ExampleStruct::stdStringG));
	// auto mapId = MIRROR_TYPE_ID(std::unordered_map<int, bool>);

	const Mirror::TypeInfo* uint8_tInfo = Mirror::InfoForType<uint8_t>();

	auto uChar8TypeId = MIRROR_TYPE_ID(unsigned char);
	auto uint8TypeId = MIRROR_TYPE_ID(uint8_t);

	ExampleStruct a;
	// Serialize::FromFile("file.txt", a);
	Serialize::ToFile(a, "file.txt");
	// #TODO Have a switch statement to check uniqueness of all generated type ids

	auto result = MIRROR_TYPE_ID(int);
	switch (uint8_tInfo->id)
	{
	case MIRROR_TYPE_ID(int32_t):
		break;

	case MIRROR_TYPE_ID(uint8_t):
		break;

	case MIRROR_TYPE_ID(uint16_t):
		break;

	case MIRROR_TYPE_ID(uint32_t):
		break;

	default:
		break;
	}

	std::map<int, char> map1;
	std::vector<int> vec1;

	std::map<int, char>::iterator mapIt;
	mapIt = map1.begin();

	std::map<int, char>* p = (std::map<int, char>*)&map1;

	mapIt = p->begin();
	auto count = p->size();

	const Mirror::TypeInfo* mapTypeInfo = Mirror::InfoForType<std::map<int, char>>();

	mapIt++;

	std::vector<int>::iterator vecIt;

	char* str = new char[sizeof(std::string)];

	new (str) std::string();
	*(std::string*)str = "ABC";
	std::string* strPtr = (std::string*)str;
	*(std::string*)str = std::string();

	strPtr = (std::string*)str;

	const Mirror::TypeInfo* baseClass = Mirror::InfoForType<ExampleClass>();
	const Mirror::TypeInfo* derived = Mirror::InfoForType<ExampleDerivedClass>();

	ExampleClass exampleClass;
	const Mirror::TypeInfo* classObjectReferenceTypeInfo = Mirror::InfoForType(exampleClass);

	ExampleStruct exampleStruct;
	const Mirror::TypeInfo* structObjectReferenceTypeInfo = Mirror::InfoForType(exampleStruct);

	int breakPoint = 0;
}

bool FileExists(const char* const filePath)
{
	FILE* file;
	if (fopen_s(&file, filePath, "r") == 0)
	{
		fclose(file);
		return true;
	}
	return false;
}

#endif
