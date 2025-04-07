
#include <bitset>
#include <iostream>
#include <string>
#include <vector>

#include "MIR_Mirror.h" // Mirror::TypeInfo, Mirror::Info, Mirror::Id

#include "Main.h"
#include "ForMirror.h" // Mirror::Id for custom types

void GettingTypeId()
{
	// Change TypeAlias and experiment
	using TypeAlias =
	int;
	// ExampleClass;
	// std::string;

	uint8_t typeId = Mirror::Info<TypeAlias>()->id; // Get ID from TypeInfo*
	// Or
	typeId = Mirror::Id<TypeAlias>(); // Explicitly specified type
	// Or
	TypeAlias object;
	typeId = Mirror::Id<>(object); // Implicitly specified type using a class object instance

	// Execute different logic depending on type
	switch (typeId)
	{
	case Mirror::Id<int>():
		std::cout << "Found an int";
		break;

	case Mirror::Id<ExampleClass>():
		{
			const Mirror::TypeInfo* typeInfo = Mirror::Info<ExampleClass>();
			std::cout << "object.intA == ";
			std::cout << ((ExampleClass*)&object)->intA;
			std::cout << "\n";

			for (int i = 0; i < typeInfo->fields.size(); i++)
			{
				std::cout << "Field: " << typeInfo->fields[i].name << ", ";
				std::cout << "Flag: " << std::bitset<8>(typeInfo->fields[i].flags) << "\n";
			}
		}
		break;

	case Mirror::Id<std::string>():
		{
			std::string* str = reinterpret_cast<std::string*>(&object);
			str->append("Hello World!");
			std::cout << str;
		}
		break;

	case Mirror::Id<void>():
		std::cout << "Example Error: void type unexpected!";
		break;
	}

	std::cout << "\n";
}
