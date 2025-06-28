
#include <bitset>
#include <iostream>
#include <string>
#include <vector>

#include "MIR_Mirror.h" // Mir::TypeInfo, Mir::Info, Mir::Id

#include "Main.h"
#include "ForMirror.h" // Mir::Id for custom types

void GettingTypeId()
{
	// Change TypeAlias and experiment
	using TypeAlias =
	int;
	// ExampleClass;
	// std::string;

	uint8_t typeId = Mir::GetInfo<TypeAlias>()->id; // Get ID from TypeInfo*
	// Or
	typeId = Mir::GetId<TypeAlias>(); // Explicitly specified type
	// Or
	TypeAlias object;
	typeId = Mir::GetId<>(object); // Implicitly specified type using a class object instance

	// Execute different logic depending on type
	switch (typeId)
	{
	case Mir::GetId<int>():
		std::cout << "Found an int";
		break;

	case Mir::GetId<ExampleClass>():
		{
			const Mir::TypeInfo* typeInfo = Mir::GetInfo<ExampleClass>();
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

	case Mir::GetId<std::string>():
		{
			std::string* str = reinterpret_cast<std::string*>(&object);
			str->append("Hello World!");
			std::cout << str;
		}
		break;

	case Mir::GetId<void>():
		std::cout << "Example Error: void type unexpected!";
		break;
	}

	std::cout << "\n";
}
