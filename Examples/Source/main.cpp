#include <cassert>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "cJSON.h"

#include "MIR_Mirror.h"
#include "MIR_ConstexprCounter.h"

#include "Main.h"
#include "MirrorTypes.h"

// #NOTE Currently unused. To enable experimental (unsafe) mirror examples
// #define MIRROR_DEV_EXPERIMENTAL

template <typename T>
void PrintClassStructure(const T& typeObj, unsigned char numTabs = 0);

int main()
{
	// #TODO Example description
	{
		// #TODO Initialization order bug. InfoForType<ExampleClass>() has not been called yet, so derived->superTypeInfo is null.
		// Could require something like MIRROR_CLASS_PARENTCLASS(Parent) in child classes, as well as parent's MIRROR_CLASS_SUBCLASS(Child)
		const Mirror::TypeInfo* intTypeInfo = Mirror::InfoForType<ExampleDerivedClass>(); // User provides type (less error prone on changes)

		// #TODO Example: Handling simple type info get and read/print (primitives, classes, inheritance, collections, pointers)
		// Getting and reading type information
		std::cout << "Printing info for type: " << intTypeInfo->stringName << "\n";
		std::cout << "Size: " << intTypeInfo->size << "\n";
		std::cout << "Id: " << intTypeInfo->id << "\n";
		std::cout << "# of fields: " << intTypeInfo->fields.size() << "\n";
		if (intTypeInfo->superTypeInfo)
		{
			std::cout << "Inherits from parent class: " << intTypeInfo->superTypeInfo->stringName << "\n";
		}

		std::vector<const Mirror::TypeInfo*> typeInfos = {
			Mirror::InfoForType<int>(),
			Mirror::InfoForType<char*>(),
			Mirror::InfoForType<std::string>(),
		};

		std::cout << "Categories:\n";
		for (size_t i = 0; i < typeInfos.size(); i++)
		{
			std::cout << typeInfos[i]->stringName;
			switch (typeInfos[i]->category)
			{
			case Mirror::TypeInfoCategory_Primitive:
				std::cout << " is a primitive" << "\n";
				break;
			case Mirror::TypeInfoCategory_Class:
				std::cout << " is a class" << "\n";
				break;
			case Mirror::TypeInfoCategory_Collection:
				std::cout << " is a collection" << "\n";
				break;
			case Mirror::TypeInfoCategory_Pair:
				std::cout << " is a pair" << "\n";
				break;
			case Mirror::TypeInfoCategory_Pointer:
				std::cout << " is a pointer" << "\n";
				break;
			}
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
