
#include <assert.h>
#include <iostream>

#include "MIR_Mirror.h" // Mir::TypeInfo, Mir::Info

#include "Main.h"

void GettingTypeInfo()
{
	const Mir::TypeInfo* typeInfo = nullptr; // Create pointer to TypeInfo struct

	typeInfo = Mir::Info<ExampleClass>(); // Explicitly specified type
	// Or
	ExampleClass object;
	typeInfo = Mir::Info<>(object); // Implicitly specified type using a class object instance

	// Simple type info data usage
	std::cout << "Printing info for type: " << typeInfo->stringName << "\n";
	std::cout << "Size: " << typeInfo->size << "\n";
	std::cout << "Id: " << static_cast<unsigned int>(typeInfo->id) << "\n";
	std::cout << "# of fields: " << typeInfo->fields.size() << "\n";

	if (typeInfo->superTypeInfo)
	{
		std::cout << "Inherits from parent class: " << typeInfo->superTypeInfo->stringName << "\n";
	}

	if (!typeInfo->derivedTypes.empty())
	{
		std::cout << "Child class(es)" << ":\n";

		for (size_t i = 0; i < typeInfo->derivedTypes.size(); i++)
		{
			std::cout << typeInfo->derivedTypes[i]->stringName << ", ";
		}
	}

	// Iterate over multiple types
	std::vector<const Mir::TypeInfo*> typeInfos = {
		Mir::Info<int>(),
		Mir::Info<char*>(),
		Mir::Info<std::string>(),
		Mir::Info<ExampleClass>(),
	};

	std::cout << "Categories:\n";
	for (size_t i = 0; i < typeInfos.size(); i++)
	{
		std::cout << typeInfos[i]->stringName;

		switch (typeInfos[i]->category)
		{
		case Mir::TypeInfoCategory_Primitive:
			std::cout << " is a primitive" << "\n";		break;
		case Mir::TypeInfoCategory_Class:
			std::cout << " is a class" << "\n";			break;
		case Mir::TypeInfoCategory_Collection:
			std::cout << " is a collection" << "\n";	break;
		case Mir::TypeInfoCategory_Pair:
			std::cout << " is a pair" << "\n";			break;
		case Mir::TypeInfoCategory_Pointer:
			std::cout << " is a pointer" << "\n";		break;
		}
	}

	std::cout << "\n";
}