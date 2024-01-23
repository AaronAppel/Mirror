
#include <stdio.h>

#include "../Libraries/cJSON/cJSON.h"

#include "Mirror.h"
#include "Serialization.h"

struct ExampleStruct
{
	int intA = 17;
	bool boolB = false;
	char charC = 'S';
	float floatD = 1.23f;
	double doubleE = 23.5607;
	const char* constCharPtrF = "Mirror";
	std::string stdStringG = "Example";
};
MIRROR_TYPE(ExampleStruct);
MIRROR_MEMBERS_FOR(ExampleStruct)
MIRROR_MEMBER(intA);
MIRROR_MEMBER(boolB);
MIRROR_MEMBER(charC);
MIRROR_MEMBER(floatD);
MIRROR_MEMBER(doubleE);
MIRROR_MEMBER(constCharPtrF);
MIRROR_MEMBER(stdStringG);
END_MEMBER_MIRROR

const char* const g_filePath = "Mirror.json";

void Serialize();
void Deserialize();

int main()
{
	Serialize();
	Deserialize();

	int breakPoint = 0;
}

void Serialize()
{
	cJSON* jsonRoot = cJSON_CreateObject();
	if (jsonRoot == nullptr)
		return;

	cJSON* arr = cJSON_CreateArray();
	arr->child = cJSON_CreateObject();
	arr->string = _strdup(Mirror::InfoForType<ExampleStruct>()->stringName.c_str());

	ExampleStruct exampleStructObj;
	Serialization::SerializeObject<ExampleStruct>(exampleStructObj, arr);

	cJSON_AddItemToArray(jsonRoot, arr);

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
