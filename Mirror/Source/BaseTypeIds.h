#pragma once

#include "Structs.h"

// #NOTE Hard coding/manually defining instead of using something like __COUNTER__ to avoid id collisions with classes

// #NOTE 0 reserved for invalid value
template <> constexpr std::size_t Mirror::TypeIdConstexpr<void>() { return 1; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<void*>() { return 2; }

// Mutable
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed char>() { return 3; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed short>() { return 4; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed int>() { return 5; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed long long>() { return 6; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned char>() { return 7; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned short>() { return 8; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned int>() { return 9; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned long long>() { return 10; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<float>() { return 11; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<double>() { return 12; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<long double>() { return 13; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<char>() { return 14; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<bool>() { return 15; }

// Const
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed char>() { return 16; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed short>() { return 17; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed int>() { return 18; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed long long>() { return 19; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned char>() { return 20; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned short>() { return 21; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned int>() { return 22; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned long long>() { return 23; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const float>() { return 24; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const double>() { return 25; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const long double>() { return 16; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const char>() { return 27; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const bool>() { return 28; }

// Mutable pointers
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed char*>() { return 29; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed short*>() { return 30; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed int*>() { return 31; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<signed long long*>() { return 32; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned char*>() { return 33; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned short*>() { return 34; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned int*>() { return 35; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<unsigned long long*>() { return 36; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<float*>() { return 37; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<double*>() { return 38; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<long double*>() { return 39; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<char*>() { return 40; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<bool*>() { return 41; }

// Const pointers
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed char*>() { return 42; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed short*>() { return 43; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed int*>() { return 44; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const signed long long*>() { return 45; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned char*>() { return 46; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned short*>() { return 47; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned int*>() { return 48; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const unsigned long long*>() { return 49; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const float*>() { return 50; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const double*>() { return 51; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const long double*>() { return 52; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const char*>() { return 53; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const bool*>() { return 54; }

// String
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::string>() { return 55; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<std::string*>() { return 56; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const std::string>() { return 57; }
template <> constexpr std::size_t Mirror::TypeIdConstexpr<const std::string*>() { return 58; }
