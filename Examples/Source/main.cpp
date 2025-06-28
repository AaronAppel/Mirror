
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
		if (Mir::GetId<std::string>() != typeInfo->fields[i].typeInfo->id &&
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
	const Mir::Info* exampleTypeInfo = Mir::GetInfo<T>();
	std::cout << "Printing class structure for type: " << exampleTypeInfo->stringName << "\n";
	PrintClassStructure(&typeObj, 1, exampleTypeInfo);
	std::cout << "\n";
}

void RecursiveClassPrint()
{
	ExampleClass exampleObject;

	// Get type information automatically using an object reference
	// Compiler detects type (changes detected automatically if type changes)
	const Mir::TypeInfo* typeInfo = Mir::GetInfo<>(exampleObject);

	// Print class type structure information using recursion
	if (typeInfo->category == Mir::TypeInfoCategory_Class)
	{
		PrintClassStructure(exampleObject, 0);
	}

	std::cout << "\n";
}

int main()
{
	//* #TODO std::Tuple support in Mirror
	std::tuple<int, char, bool, int, char, bool> tuple6 = std::make_tuple(0, 1, 0, 0, 0, 0);
	auto var = &std::get<1>(tuple6);

	constexpr size_t sizeT = 0;
	auto varSize = sizeof(std::get<sizeT>(tuple6));
	varSize = sizeof(std::get<true>(tuple6));
	auto val = sizeof(std::get<true>(tuple6));

	auto var1 = &std::get<0>(tuple6);
	auto var2 = &std::get<1>(tuple6);

	// #TODO Broken Mirror::IdForType(tuple6);
	// Mirror::IdForType<std::tuple<int, char, bool, int, char, bool>>();
	// Mirror::InfoForType(tuple6);
	//*/

	GettingTypeInfo();
	GettingTypeId();
	FileIO();
	RecursiveClassPrint();
	TypesAsArguments();

	// #TODO Constructors, and construction using member variables or passing other arguments to constructors
	// #TODO Class inheritance relational reflection, showing how to connect parent and child class infos, and how to find and use reflected meta data

	system("pause");
}
