
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

#include <tuple>
#include <iostream>
#include <typeinfo>
#include <array>
#include <functional>

template <typename Tuple, std::size_t... Is>
auto make_address_getters(std::index_sequence<Is...>) {
	using getter_t = std::function<char* (Tuple&)>;
	return std::array<getter_t, sizeof...(Is)>{
		{ [](Tuple& t) -> char* {
			return reinterpret_cast<char*>(&std::get<Is>(t));
			}... }
	};
}

template <typename... Args>
char* get_tuple_member_address_by_index(std::tuple<Args...>& t, std::size_t index) {
	static auto getters = make_address_getters<std::tuple<Args...>>(std::index_sequence_for<Args...>{});
	if (index >= getters.size()) {
		return nullptr;
	}
	return getters[index](t);
}

#include <array>
int main()
{
	std::tuple<int, double, char> tup{ 123, 3.14, 'A' };

	std::tuple<int, double, char>& tupRef = tup;
	get_tuple_member_address_by_index(tupRef, 0);

	auto v = make_address_getters<std::tuple<int, double, char>>(std::index_sequence_for<int, double, char>{});

	for (std::size_t i = 0; i < 3; ++i) {
		char* addr = get_tuple_member_address_by_index(tup, i);
		std::cout << "Element #" << i << " address: " << static_cast<void*>(addr) << "\n";
	}

	// Modify the first element (int) to 456
	if (char* addr = get_tuple_member_address_by_index(tup, 0)) {
		*reinterpret_cast<int*>(addr) = 456;
	}

	// Modify the third element (char) to 'Z'
	if (char* addr = get_tuple_member_address_by_index(tup, 2)) {
		*reinterpret_cast<char*>(addr) = 'Z';
	}

	// Print updated tuple values
	std::cout << "Updated values:\n";
	std::cout << "int: " << std::get<0>(tup) << "\n";
	std::cout << "double: " << std::get<1>(tup) << "\n";
	std::cout << "char: " << std::get<2>(tup) << "\n";

	// #TODO Test std::array by de/serializing
	std::array<int, 10> arr;
	arr[0] = 0;
	size_t index = 0;
	if (index < arr.size())
	{
	}
	// #TODO Linker error, can't find IdForType(obj), but GetId might work
	Mir::Id arrayInt10Id = Mir::IdForType<std::array<int, 10>>();
	Mir::IdForType(arr);
	Mir::GetId<std::array<int, 10>>();

	std::tuple<int, bool, char, double, float> tupleMulti = std::make_tuple(0, 0, 0, 0, 0);
	const size_t tupleIndex = 0;
	auto varX = std::get<tupleIndex>(tupleMulti);
	int c = varX;
	std::cout << c;

	Mir::Id tupleMultiId = Mir::GetId(tupleMulti);
	const Mir::TypeInfo* tupleMultiInfo = Mir::GetInfo(tupleMulti);

	char* intAddress = tupleMultiInfo->collectionIterateCurrentFunc(&tupleMulti, 0);
	*(int*)(intAddress) = 1234;

	char* boolAddress = tupleMultiInfo->collectionIterateCurrentFunc(&tupleMulti, 1);
	*(bool*)(boolAddress) = true;

	char* charAddress = tupleMultiInfo->collectionIterateCurrentFunc(&tupleMulti, 2);
	*charAddress = 'A';

	char* doubleAddress = tupleMultiInfo->collectionIterateCurrentFunc(&tupleMulti, 3);
	*(double*)(doubleAddress) = 1.5;

	char* floatAddress = tupleMultiInfo->collectionIterateCurrentFunc(&tupleMulti, 4);
	*(float*)(floatAddress) = 5.76f;

	std::tuple<int> tupleInt = std::make_tuple(0);
	Mir::Id tupleIntId = Mir::GetId(tupleInt);
	const Mir::TypeInfo* tupleIntInfo = Mir::GetInfo(tupleInt);

	// Mir::Id intId = Mir::GetId<int>();

	// #TODO IdForType not working here, but does in QwerkE as _MSVC_TRADITIONAL is not defined. Using GetId() instead
	// Mir::Id tupleVecIntId = Mir::IdForType(tupleVecInt);

	//* #TODO std::Tuple support in Mirror
	std::tuple<int, char, bool, int, char, bool> tuple6 = std::make_tuple(0, 1, 0, 0, 0, 0);
	auto var = &std::get<1>(tuple6);

	// Mir::Id tupleId = Mir::IdForType(tuple6);

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

	// #TODO Mirroring user defined collections

	system("pause");
}
