
#ifdef MIRROR_EXAMPLES
#include "main.h"

#include <iostream>
#include <stdio.h>

#include "../../cJSON/Source/cJSON.h"

#include "Mirror.h"
#include "Serialization/Serialization.h"

const char* const g_filePath = "Mirror.json";

bool FileExists();

void Serialize();
void Deserialize();

int main()
{
	const Mirror::TypeInfo* baseClass = Mirror::InfoForType<ExampleClass>();
	const Mirror::TypeInfo* derived = Mirror::InfoForType<ExampleDerivedClass>();

	ExampleClass exampleClass;
	const Mirror::TypeInfo* classObjectReferenceTypeInfo = Mirror::InfoForType(exampleClass);

	ExampleStruct exampleStruct;
	const Mirror::TypeInfo* structObjectReferenceTypeInfo = Mirror::InfoForType(exampleStruct);

	Serialize();
	Deserialize();

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
	if (!FileExists(g_filePath))
	{
		std::cout << "File not found: " << g_filePath;
		return;
	}

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
#endif
