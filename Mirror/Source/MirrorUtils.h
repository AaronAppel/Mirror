#pragma once

// #TODO Find a way to generate hash at compile time for more use cases

#define PREDEFINED_ID_MAX 60
constexpr std::size_t HashFromString(const char* type_name)
{
	// #TODO Improve naive pseudo-unique output implementation
	std::size_t result { PREDEFINED_ID_MAX + 1 };
	const char* charPtr = type_name;
	while (*charPtr != '\0')
	{
		result += *charPtr;
		charPtr += 1;
	}

	return result;
}

template <typename T>
constexpr std::size_t HashFromString(const char* type_name)
{
	return HashFromString(type_name);
}

// #TODO Look to use a macro an __COUNTER__ to avoid manually implementing each type and id value
// #TODO Incorporate a static assert with PREDEFINED_ID_MAX

// Hash 0 reserved for now

// Mutable
template <> constexpr std::size_t HashFromString<signed char>(const char* type_name) { return 1; }
template <> constexpr std::size_t HashFromString<signed short>(const char* type_name) { return 2; }
template <> constexpr std::size_t HashFromString<signed int>(const char* type_name) { return 3; }
template <> constexpr std::size_t HashFromString<signed long long>(const char* type_name) { return 4; }

template <> constexpr std::size_t HashFromString<unsigned char>(const char* type_name) { return 5; }
template <> constexpr std::size_t HashFromString<unsigned short>(const char* type_name) { return 6; }
template <> constexpr std::size_t HashFromString<unsigned int>(const char* type_name) { return 7; }
template <> constexpr std::size_t HashFromString<unsigned long long>(const char* type_name) { return 8; }

template <> constexpr std::size_t HashFromString<float>(const char* type_name) { return 9; }
template <> constexpr std::size_t HashFromString<double>(const char* type_name) { return 10; }
template <> constexpr std::size_t HashFromString<long double>(const char* type_name) { return 11; }

template <> constexpr std::size_t HashFromString<char>(const char* type_name) { return 12; }

template <> constexpr std::size_t HashFromString<bool>(const char* type_name) { return 13 ; }

// Const
template <> constexpr std::size_t HashFromString<const signed char>(const char* type_name) { return 14; }
template <> constexpr std::size_t HashFromString<const signed short>(const char* type_name) { return 15; }
template <> constexpr std::size_t HashFromString<const signed int>(const char* type_name) { return 16; }
template <> constexpr std::size_t HashFromString<const signed long long>(const char* type_name) { return 17; }

template <> constexpr std::size_t HashFromString<const unsigned char>(const char* type_name) { return 18; }
template <> constexpr std::size_t HashFromString<const unsigned short>(const char* type_name) { return 19; }
template <> constexpr std::size_t HashFromString<const unsigned int>(const char* type_name) { return 20; }
template <> constexpr std::size_t HashFromString<const unsigned long long>(const char* type_name) { return 21; }

template <> constexpr std::size_t HashFromString<const float>(const char* type_name) { return 22; }
template <> constexpr std::size_t HashFromString<const double>(const char* type_name) { return 23; }
template <> constexpr std::size_t HashFromString<const long double>(const char* type_name) { return 14; }

template <> constexpr std::size_t HashFromString<const char>(const char* type_name) { return 25; }

template <> constexpr std::size_t HashFromString<const bool>(const char* type_name) { return 26; }

// Mutable pointers
template <> constexpr std::size_t HashFromString<signed char*>(const char* type_name) { return 27; }
template <> constexpr std::size_t HashFromString<signed short*>(const char* type_name) { return 28; }
template <> constexpr std::size_t HashFromString<signed int*>(const char* type_name) { return 29; }
template <> constexpr std::size_t HashFromString<signed long long*>(const char* type_name) { return 30; }

template <> constexpr std::size_t HashFromString<unsigned char*>(const char* type_name) { return 31; }
template <> constexpr std::size_t HashFromString<unsigned short*>(const char* type_name) { return 32; }
template <> constexpr std::size_t HashFromString<unsigned int*>(const char* type_name) { return 33; }
template <> constexpr std::size_t HashFromString<unsigned long long*>(const char* type_name) { return 34; }

template <> constexpr std::size_t HashFromString<float*>(const char* type_name) { return 35; }
template <> constexpr std::size_t HashFromString<double*>(const char* type_name) { return 36; }
template <> constexpr std::size_t HashFromString<long double*>(const char* type_name) { return 37; }

template <> constexpr std::size_t HashFromString<char*>(const char* type_name) { return 38; }

template <> constexpr std::size_t HashFromString<bool*>(const char* type_name) { return 39; }

// Const pointers
template <> constexpr std::size_t HashFromString<const signed char*>(const char* type_name) { return 40; }
template <> constexpr std::size_t HashFromString<const signed short*>(const char* type_name) { return 41; }
template <> constexpr std::size_t HashFromString<const signed int*>(const char* type_name) { return 42; }
template <> constexpr std::size_t HashFromString<const signed long long*>(const char* type_name) { return 43; }

template <> constexpr std::size_t HashFromString<const unsigned char*>(const char* type_name) { return 44; }
template <> constexpr std::size_t HashFromString<const unsigned short*>(const char* type_name) { return 45; }
template <> constexpr std::size_t HashFromString<const unsigned int*>(const char* type_name) { return 46; }
template <> constexpr std::size_t HashFromString<const unsigned long long*>(const char* type_name) { return 47; }

template <> constexpr std::size_t HashFromString<const float*>(const char* type_name) { return 48; }
template <> constexpr std::size_t HashFromString<const double*>(const char* type_name) { return 49; }
template <> constexpr std::size_t HashFromString<const long double*>(const char* type_name) { return 50; }

template <> constexpr std::size_t HashFromString<const char*>(const char* type_name) { return 51; }

template <> constexpr std::size_t HashFromString<const bool*>(const char* type_name) { return 52; }

//