#include <cassert>
#include <iostream>
#include <stdio.h>
#include <string>

#include "cJSON.h"
#include "MIR_Mirror.h"
#include "MIR_ConstexprCounter.h"

#include "main.h"
#include "MirrorTypes.h"

// #NOTE Currently unused. To enable experimental (unsafe) mirror examples
// #define MIRROR_DEV_EXPERIMENTAL

template <typename T>
void PrintClassStructure(const T& typeObj, unsigned char numTabs = 0);

int main()
{
	// #TODO Example description
	{
		const Mirror::TypeInfo* intTypeInfo = Mirror::InfoForType<std::string>(); // User provides type (less error prone on changes)

		// #TODO Example: Handling simple type info get and read/print (primitives, classes, inheritance, collections, pointers)
		// Getting and reading type information
		std::cout << "Printing info for type: " << intTypeInfo->stringName << "\n";
		std::cout << "Size: " << intTypeInfo->size << "\n";
		std::cout << "Id: " << intTypeInfo->id << "\n";

		std::cout << "Category: ";
		switch (intTypeInfo->category)
		{
		case Mirror::TypeInfoCategory_Primitive:
			std::cout << "primitive" << "\n";
			break;
		case Mirror::TypeInfoCategory_Class:
			std::cout << "class" << "\n";
			break;
		case Mirror::TypeInfoCategory_Collection:
			std::cout << "collection" << "\n";
			break;
		case Mirror::TypeInfoCategory_Pair:
			std::cout << "pair" << "\n";
			break;
		case Mirror::TypeInfoCategory_Pointer:
			std::cout << "pointer" << "\n";
			break;
		}
	}

	// #TODO Example description
	{
		int exampleObject;
		// ExampleClass exampleObject;
		// ExampleDerivedClass exampleObject;

		const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<>(exampleObject);

		switch (typeInfo->id)
		{
		case Mirror::TypeId<int>():
			std::cout << "Found an int\n";
			break;

		case Mirror::TypeId<std::string>():
		case Mirror::TypeId<const char*>():
			std::cout << "Found a string or const char*\n";
			break;

		case Mirror::TypeId<void>():
			std::cout << "Error: void type unexpected!\n";
			break;

		case Mirror::TypeId<float[10]>():
			std::cout << "Found a float array\n";
			break;

		case Mirror::TypeId<ExampleDerivedClass>():
		case Mirror::TypeId<ExampleClass>():
			std::cout << "Found an ExampleClass\n";
			std::cout << ((ExampleClass*)&exampleObject)->intA;
			std::cout << "\n";
			break;
		}
	}

	// #TODO Example description
	{
		// ExampleNestedCustomTypes exampleObject;
		ExampleClass exampleObject;
		// ExampleDerivedClass exampleObject;

		// Get type information automatically using an object reference
		const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<>(exampleObject); // Compiler detects type (changes detected automatically when type changes)

		// Print class type structure information using recursion
		if (typeInfo->category == Mirror::TypeInfoCategory_Class)
		{
			PrintClassStructure(exampleObject, 0);
		}
	}

	system("pause");
}

void PrintClassStructure(const void* typeObj, unsigned char numTabs, const Mirror::TypeInfo* typeInfo)
{
	for (size_t i = 0; i < typeInfo->fields.size(); i++)
	{
		for (size_t i = 0; i < numTabs; i++)
		{
			std::cout << "  ";
		}

		std::cout << typeInfo->fields[i].name;

		if (typeInfo->fields[i].typeInfo->category == Mirror::TypeInfoCategory_Class)
		{
			std::cout << ": " << typeInfo->fields[i].typeInfo->stringName << "\n";
			PrintClassStructure(((char*)typeObj) + typeInfo->fields[i].offset, numTabs + 1, typeInfo->fields[i].typeInfo);
		}

		if (i < typeInfo->fields.size() - 1)
		{
			std::cout << "\n";
		}
	}
}

template <typename T>
void PrintClassStructure(const T& typeObj, unsigned char numTabs)
{
	std::cout << "\n";
	const Mirror::TypeInfo* exampleTypeInfo = Mirror::InfoForType<T>();
	std::cout << "Printing class structure for type: " << exampleTypeInfo->stringName << "\n";
	PrintClassStructure(&typeObj, 1, exampleTypeInfo);
	std::cout << "\n";
}
