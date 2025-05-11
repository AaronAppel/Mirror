#pragma once

#include <map>
#include <string>

#define MIRROR_PRIVATE_MEMBERS friend struct Mir;

void GettingTypeInfo();
void GettingTypeId();
void FileIO();
void RecursiveClassPrint();
void TypesAsArguments();

class ExampleParentClass
{
};

class ExampleClass : public ExampleParentClass
{
public:
	int intA = 17;
	bool boolB = false;

private:
	MIRROR_PRIVATE_MEMBERS
		char charC = 'S';
	float floatD = 1.23f;
	double doubleE = 23.5607;
	const char* constCharPtrF = "Mirror";
	std::string stdStringG = "Example";
	std::map<int, bool> exampleMapH = { { 0, true } };
};

class ExampleChildClass : public ExampleClass
{
private:
	MIRROR_PRIVATE_MEMBERS
		int intZ = 7;
};

struct ExampleNestedCustomTypes
{
	ExampleClass exClass;
};
