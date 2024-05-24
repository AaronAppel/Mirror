#pragma once

// #TODO Find a way to generate hash at comple time for more cases
// template <typename T>
// constexpr std::size_t HashFromString(const char* type_name) { }

constexpr std::size_t HashFromString(const char* type_name)
{
	// #TODO Improve naive pseudo-unique output implementation
	std::size_t result{ 0xf };
	const char* charPtr = type_name;
	while (*charPtr != '\0')
	{
		result += *charPtr;
		charPtr += 1;
	}

	return result;
}
