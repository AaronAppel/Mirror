
#include <cassert>
#include <iostream>
#include <stdio.h>
#include <string>

#include "MIR_Mirror.h"

#include "main_testing.h"
#include "MirrorTypeIds_Testing.h"

int main()
{
	// Exhaustive type checking to catch compile and link errors
	Mirror::InfoForType<void>();
	Mirror::InfoForType<void*>();

	Mirror::InfoForType<signed char>();
	Mirror::InfoForType<signed short>();
	Mirror::InfoForType<signed int>();
	Mirror::InfoForType<signed long long>();
	Mirror::InfoForType<unsigned char>();
	Mirror::InfoForType<unsigned short>();
	Mirror::InfoForType<unsigned int>();
	Mirror::InfoForType<unsigned long long>();
	Mirror::InfoForType<float>();
	Mirror::InfoForType<double>();
	Mirror::InfoForType<long double>();
	Mirror::InfoForType<char>();
	Mirror::InfoForType<bool>();

	Mirror::InfoForType<const signed char>();
	Mirror::InfoForType<const signed short>();
	Mirror::InfoForType<const signed int>();
	Mirror::InfoForType<const signed long long>();
	Mirror::InfoForType<const unsigned char>();
	Mirror::InfoForType<const unsigned short>();
	Mirror::InfoForType<const unsigned int>();
	Mirror::InfoForType<const unsigned long long>();
	Mirror::InfoForType<const float>();
	Mirror::InfoForType<const double>();
	Mirror::InfoForType<const long double>();
	Mirror::InfoForType<const char>();
	Mirror::InfoForType<const bool>();

	Mirror::InfoForType<signed char*>();
	Mirror::InfoForType<signed short*>();
	Mirror::InfoForType<signed int*>();
	Mirror::InfoForType<signed long long*>();
	Mirror::InfoForType<unsigned char*>();
	Mirror::InfoForType<unsigned short*>();
	Mirror::InfoForType<unsigned int*>();
	Mirror::InfoForType<unsigned long long*>();
	Mirror::InfoForType<float*>();
	Mirror::InfoForType<double*>();
	Mirror::InfoForType<long double*>();
	Mirror::InfoForType<char*>();
	Mirror::InfoForType<bool*>();

	Mirror::InfoForType<const signed char*>();
	Mirror::InfoForType<const signed short*>();
	Mirror::InfoForType<const signed int*>();
	Mirror::InfoForType<const signed long long*>();
	Mirror::InfoForType<const unsigned char*>();
	Mirror::InfoForType<const unsigned short*>();
	Mirror::InfoForType<const unsigned int*>();
	Mirror::InfoForType<const unsigned long long*>();
	Mirror::InfoForType<const float*>();
	Mirror::InfoForType<const double*>();
	Mirror::InfoForType<const long double*>();
	Mirror::InfoForType<const char*>();
	Mirror::InfoForType<const bool*>();

	Mirror::InfoForType<std::string>();
	Mirror::InfoForType<std::string*>();
	Mirror::InfoForType<const std::string>();
	Mirror::InfoForType<const std::string*>();

	Mirror::InfoForType<float[10]>();

	Mirror::InfoForType<std::vector<char>>();

	Mirror::InfoForType<std::pair<const int, bool>>();
	Mirror::InfoForType<std::pair<std::string, int32_t>>();
	// #TODO Problems in SetConstructionLambda() Mirror::InfoForType<std::pair<const std::string, int32_t>>();

	Mirror::InfoForType<std::map<int, bool>>();
	Mirror::InfoForType<std::unordered_map<std::string, int32_t>>();
	// #TODO Problems in SetConstructionLambda() Mirror::InfoForType<std::unordered_map<const std::string, int32_t>>();

	Mirror::InfoForType<Derived1>();
	Mirror::InfoForType<Derived1*>();
	Mirror::InfoForType<Derived2>();
	Mirror::InfoForType<Derived2*>();
	Mirror::InfoForType<Base>();
	Mirror::InfoForType<Base*>();
	Mirror::InfoForType<TestStruct>();
	Mirror::InfoForType<TestStruct*>();

	Mirror::InfoForType<ExampleStruct>();
	Mirror::InfoForType<ExampleClass>();
	Mirror::InfoForType<ExampleDerivedClass>();
	Mirror::InfoForType<ExampleNestedCustomTypes>();

	// #TODO Exhaustive TypeId constexpr generation check using a switch statement
	constexpr int typeId = 0; // #TODO Choose proper case
	switch (typeId)
	{
		case Mirror::TypeId<void>():
		case Mirror::TypeId<void*>():

		case Mirror::TypeId<signed char>():
		case Mirror::TypeId<signed short>():
		case Mirror::TypeId<signed int>():
		case Mirror::TypeId<signed long long>():
		case Mirror::TypeId<unsigned char>():
		case Mirror::TypeId<unsigned short>():
		case Mirror::TypeId<unsigned int>():
		case Mirror::TypeId<unsigned long long>():
		case Mirror::TypeId<float>():
		case Mirror::TypeId<double>():
		case Mirror::TypeId<long double>():
		case Mirror::TypeId<char>():
		case Mirror::TypeId<bool>():

		case Mirror::TypeId<const signed char>():
		case Mirror::TypeId<const signed short>():
		case Mirror::TypeId<const signed int>():
		case Mirror::TypeId<const signed long long>():
		case Mirror::TypeId<const unsigned char>():
		case Mirror::TypeId<const unsigned short>():
		case Mirror::TypeId<const unsigned int>():
		case Mirror::TypeId<const unsigned long long>():
		case Mirror::TypeId<const float>():
		case Mirror::TypeId<const double>():
		case Mirror::TypeId<const long double>():
		case Mirror::TypeId<const char>():
		case Mirror::TypeId<const bool>():

		case Mirror::TypeId<signed char*>():
		case Mirror::TypeId<signed short*>():
		case Mirror::TypeId<signed int*>():
		case Mirror::TypeId<signed long long*>():
		case Mirror::TypeId<unsigned char*>():
		case Mirror::TypeId<unsigned short*>():
		case Mirror::TypeId<unsigned int*>():
		case Mirror::TypeId<unsigned long long*>():
		case Mirror::TypeId<float*>():
		case Mirror::TypeId<double*>():
		case Mirror::TypeId<long double*>():
		case Mirror::TypeId<char*>():
		case Mirror::TypeId<bool*>():

		case Mirror::TypeId<const signed char*>():
		case Mirror::TypeId<const signed short*>():
		case Mirror::TypeId<const signed int*>():
		case Mirror::TypeId<const signed long long*>():
		case Mirror::TypeId<const unsigned char*>():
		case Mirror::TypeId<const unsigned short*>():
		case Mirror::TypeId<const unsigned int*>():
		case Mirror::TypeId<const unsigned long long*>():
		case Mirror::TypeId<const float*>():
		case Mirror::TypeId<const double*>():
		case Mirror::TypeId<const long double*>():
		case Mirror::TypeId<const char*>():
		case Mirror::TypeId<const bool*>():

		case Mirror::TypeId<std::string>():
		case Mirror::TypeId<std::string*>():
		case Mirror::TypeId<const std::string>():
		case Mirror::TypeId<const std::string*>():

		case Mirror::TypeId<float[10]>():

		case Mirror::TypeId<std::vector<char>>():

		case Mirror::TypeId<std::pair<const int, bool>>():
		case Mirror::TypeId<std::pair<std::string, int32_t>>():
		case Mirror::TypeId<std::pair<const std::string, int32_t>>():

		case Mirror::TypeId<std::map<int, bool>>():
			// #TODO Problems in SetConstructionLambda() case Mirror::TypeId<std::unordered_map<std::string, int32_t>>():

		case Mirror::TypeId<ExampleClass>():
		case Mirror::TypeId<ExampleDerivedClass>():
		default:
			break;
	}

	// #TODO React to case pass/fail results
}
