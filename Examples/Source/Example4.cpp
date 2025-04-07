
#include <iostream>

#include "MIR_Mirror.h" // Mir::TypeInfo, Mir::Info

#include "Main.h"

template <typename... Type>
void ExecuteOnSingleType()
{
	([] {
		// Execute logic per Type
		const Mir::TypeInfo* typeInfo = Mir::Info<Type>();

		std::cout << "Typename: " << typeInfo->stringName << "\n";
		if (Mir::TypeInfoCategory_Class == typeInfo->category)
		{
			std::cout << "Class fields:\n";
			for (int i = 0; i < typeInfo->fields.size(); i++)
			{
				std::cout << "  " << typeInfo->fields[i].name << " is a " << typeInfo->fields[i].typeInfo->stringName << "\n";
			}
			std::cout << "\n";
		}
		else
		{
			std::cout << "Has no fields\n\n";
		}
	}(), ...);
}

template<typename... Types>
void ExecuteOnMultipleTypes(Mir::TypesList<Types...>)
{
	// Pass types expanded
	ExecuteOnSingleType<Types...>();
}

void TypesAsArguments()
{
	// Create a list of types
	using Types = Mir::TypesList <
		int,
		ExampleClass,
		ExampleChildClass
	>;

	// Pass types as a list of arguments to a function
	ExecuteOnMultipleTypes(Types{});

	std::cout << "\n";
}