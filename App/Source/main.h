#pragma once

#ifdef MIRROR_EXAMPLES
#include <string>

#include "Mirror.h"

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

// TODO Serialization layouts. Design the user implementation example for Mirror
struct ExampleNestedCutomTypes
{
	ExampleStruct exStruct;
	ExampleClass exClass;
};
#endif