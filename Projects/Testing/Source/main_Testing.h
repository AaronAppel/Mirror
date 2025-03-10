#pragma once

#include <map>
#include <string>

#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

struct ExampleStruct
{
	int intA = 17;
	bool boolB = false;
	char charC = 'S';
	float floatD = 1.23f;
	double doubleE = 23.5607;
	const char* constCharPtrF = "Mirror";
	std::string stdStringG = "Example";
	std::map<int, bool> exampleMapH = { { 0, true } };
};

class ExampleClass
{
public:
	int intX = 9;

private:
	MIRROR_PRIVATE_MEMBERS
	int intY = 4;
};

class ExampleDerivedClass : public ExampleClass
{
private:
	MIRROR_PRIVATE_MEMBERS
		int intZ = 7;
};

// #TODO Serialization layouts. Design the user implementation example for Mirror
struct ExampleNestedCustomTypes
{
	ExampleStruct exStruct;
	ExampleClass exClass;
	ExampleDerivedClass exDerivedClass;
};