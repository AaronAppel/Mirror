#pragma once

#include <map>
#include <string>

#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

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

// #TODO Templated collection example
class UserCollection
{
public:
	UserCollection() = default;

	int* At(int index)
	{
		if (index < size)
		{
			return &buffer[index];
		}
		return nullptr;
	}

	void Add(int value)
	{
		if (nextIndex < size)
		{
			buffer[nextIndex] = value;
		}
	}

	int* Data()
	{
		return &buffer[0];
	}

	int* End()
	{
		return &buffer[0] + size;
	}

private:
	MIRROR_PRIVATE_MEMBERS

	static const int size = 10;
	int buffer[size];
	int nextIndex = 0;
};
