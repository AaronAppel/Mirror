#pragma once

#include <string>

#include "Structs.h"

// #NOTE Hard coding/manually defining instead of using something like __COUNTER__ to avoid id collisions with classes
#define DECLARE_TYPE_ID(TYPE, ID) \
template <> constexpr std::size_t Mirror::TypeId<TYPE>() { return ID; }

// #NOTE 0 reserved for invalid value
DECLARE_TYPE_ID(void, 1)
DECLARE_TYPE_ID(void*, 2)

// Mutable
DECLARE_TYPE_ID(signed char, 3)
DECLARE_TYPE_ID(signed short, 4)
DECLARE_TYPE_ID(signed int, 5)
DECLARE_TYPE_ID(signed long long, 6)
DECLARE_TYPE_ID(unsigned char, 7)
DECLARE_TYPE_ID(unsigned short, 8)
DECLARE_TYPE_ID(unsigned int, 9)
DECLARE_TYPE_ID(unsigned long long, 10)
DECLARE_TYPE_ID(float, 11)
DECLARE_TYPE_ID(double, 12)
DECLARE_TYPE_ID(long double, 13)
DECLARE_TYPE_ID(char, 14)
DECLARE_TYPE_ID(bool, 15)

// Const
DECLARE_TYPE_ID(const signed char, 16)
DECLARE_TYPE_ID(const signed short, 17)
DECLARE_TYPE_ID(const signed int, 18)
DECLARE_TYPE_ID(const signed long long, 19)
DECLARE_TYPE_ID(const unsigned char, 20)
DECLARE_TYPE_ID(const unsigned short, 21)
DECLARE_TYPE_ID(const unsigned int, 22)
DECLARE_TYPE_ID(const unsigned long long, 23)
DECLARE_TYPE_ID(const float, 24)
DECLARE_TYPE_ID(const double, 25)
DECLARE_TYPE_ID(const long double, 26)
DECLARE_TYPE_ID(const char, 27)
DECLARE_TYPE_ID(const bool, 28)

// Mutable pointers
DECLARE_TYPE_ID(signed char*, 29)
DECLARE_TYPE_ID(signed short*, 30)
DECLARE_TYPE_ID(signed int*, 31)
DECLARE_TYPE_ID(signed long long*, 32)
DECLARE_TYPE_ID(unsigned char*, 33)
DECLARE_TYPE_ID(unsigned short*, 34)
DECLARE_TYPE_ID(unsigned int*, 35)
DECLARE_TYPE_ID(unsigned long long*, 36)
DECLARE_TYPE_ID(float*, 37)
DECLARE_TYPE_ID(double*, 38)
DECLARE_TYPE_ID(long double*, 39)
DECLARE_TYPE_ID(char*, 40)
DECLARE_TYPE_ID(bool*, 41)

// Const pointers
DECLARE_TYPE_ID(const signed char*, 42)
DECLARE_TYPE_ID(const signed short*, 43)
DECLARE_TYPE_ID(const signed int*, 44)
DECLARE_TYPE_ID(const signed long long*, 45)
DECLARE_TYPE_ID(const unsigned char*, 46)
DECLARE_TYPE_ID(const unsigned short*, 47)
DECLARE_TYPE_ID(const unsigned int*, 48)
DECLARE_TYPE_ID(const unsigned long long*, 49)
DECLARE_TYPE_ID(const float*, 50)
DECLARE_TYPE_ID(const double*, 51)
DECLARE_TYPE_ID(const long double*, 52)
DECLARE_TYPE_ID(const char*, 53)
DECLARE_TYPE_ID(const bool*, 54)

// String
DECLARE_TYPE_ID(std::string, 55)
DECLARE_TYPE_ID(std::string*, 56)
DECLARE_TYPE_ID(const std::string, 57)
DECLARE_TYPE_ID(const std::string*, 58)
