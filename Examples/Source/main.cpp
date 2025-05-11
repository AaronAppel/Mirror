
#include <iostream>
#include <stdio.h>
#include <string>

#include "cJSON.h"

#include "MIR_Mirror.h"

#include "Main.h"
#include "ForMirror.h"

void PrintClassStructure(const void* typeObj, unsigned char numTabs, const Mir::TypeInfo* typeInfo)
{
	for (size_t i = 0; i < typeInfo->fields.size(); i++)
	{
		for (size_t i = 0; i < numTabs; i++)
		{
			std::cout << "  ";
		}

		std::cout << typeInfo->fields[i].name;

		// #TODO std::string type not a class
		if (Mir::Id<std::string>() != typeInfo->fields[i].typeInfo->id &&
			typeInfo->fields[i].typeInfo->category == Mir::TypeInfoCategory_Class)
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
	const Mir::TypeInfo* exampleTypeInfo = Mir::Info<T>();
	std::cout << "Printing class structure for type: " << exampleTypeInfo->stringName << "\n";
	PrintClassStructure(&typeObj, 1, exampleTypeInfo);
	std::cout << "\n";
}

void RecursiveClassPrint()
{
	ExampleClass exampleObject;

	// Get type information automatically using an object reference
	// Compiler detects type (changes detected automatically if type changes)
	const Mir::TypeInfo* typeInfo = Mir::Info<>(exampleObject);

	// Print class type structure information using recursion
	if (typeInfo->category == Mir::TypeInfoCategory_Class)
	{
		PrintClassStructure(exampleObject, 0);
	}

	std::cout << "\n";
}

int main()
{
	GettingTypeInfo();
	GettingTypeId();
	FileIO();
	RecursiveClassPrint();
	TypesAsArguments();

	system("pause");
}
