#include "main.h"

#include <stdio.h>

#include "../Libraries/cJSON/cJSON.h"

#include "Mirror.h"
#include "Serialization.h"

MIRROR_CLASS_START(ExampleStruct)
MIRROR_CLASS_MEMBER(intA);
MIRROR_CLASS_MEMBER(boolB);
MIRROR_CLASS_MEMBER(charC);
MIRROR_CLASS_MEMBER(floatD);
MIRROR_CLASS_MEMBER(doubleE);
MIRROR_CLASS_MEMBER(constCharPtrF);
MIRROR_CLASS_MEMBER(stdStringG);
MIRROR_CLASS_END

MIRROR_CLASS_START(ExampleClass)
MIRROR_CLASS_MEMBER(intX)
MIRROR_CLASS_MEMBER(intY)
MIRROR_CLASS_END

MIRROR_CLASS_START(ExampleNestedCutomTypes)
MIRROR_CLASS_MEMBER(exStruct)
MIRROR_CLASS_MEMBER(exClass)
MIRROR_CLASS_END

const char* const g_filePath = "Mirror.json";

void Serialize();
void Deserialize();

int main()
{
	std::vector<Mirror::Field> ints;
	ints.reserve(10);
	Mirror::Field field;
	field.name = "";
	ints.push_back(field);

	const Mirror::ClassInfo* a = Mirror::InfoForClass<ExampleClass>();

	ExampleClass exClass;
	const Mirror::ClassInfo* b = Mirror::InfoForClass(exClass);

	ExampleStruct exStruct;
	const Mirror::ClassInfo* c = Mirror::InfoForClass(exStruct);

	Serialize();
	// Deserialize();

	int breakPoint = 0;
}

void Serialize()
{
	cJSON* jsonRoot = cJSON_CreateObject();
	if (jsonRoot == nullptr)
		return;

	ExampleStruct exampleStructObj;
	Serialization::SerializeObject<ExampleStruct>(exampleStructObj, jsonRoot);

	ExampleClass exampleClassObj;
	Serialization::SerializeObject<ExampleClass>(exampleClassObj, jsonRoot);

	int8_t exampleInt = 2;
	Serialization::SerializeType<int8_t>(exampleInt, jsonRoot);

	ExampleNestedCutomTypes exampleNestedCutomTypesObj;
	Serialization::SerializeObject<ExampleNestedCutomTypes>(exampleNestedCutomTypesObj, jsonRoot);

	const char* jsonStr = cJSON_Print(jsonRoot);
	if (jsonStr)
	{
		FILE* filehandle;
		errno_t error = fopen_s(&filehandle, g_filePath, "w+");
		if (filehandle)
		{
			fwrite(jsonStr, 1, strlen(jsonStr), filehandle);
			fclose(filehandle);
		}
		free((char*)jsonStr);
	}
}

void Deserialize()
{
	FILE* filehandle;
	fopen_s(&filehandle, g_filePath, "rb");
	if (filehandle)
	{
		fseek(filehandle, 0, SEEK_END);
		long size = ftell(filehandle);
		rewind(filehandle);
		char* fileBuffer = new char[size];
		fread(fileBuffer, size, 1, filehandle);
		fclose(filehandle);

		cJSON* jsonRoot = cJSON_Parse(fileBuffer);
		delete[] fileBuffer;

		if (jsonRoot == nullptr)
			return;

		ExampleStruct exampleStructObj;
		Serialization::DeserializeJsonObject<ExampleStruct>(jsonRoot->child, exampleStructObj);

		cJSON_Delete((cJSON*)jsonRoot);
	}
}
