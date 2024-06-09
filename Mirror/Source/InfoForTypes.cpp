#include "Mirror.h"

// Void
// #TODO Clean up void specific (avoid sizeof()) implementation
template<> static const Mirror::TypeInfo* Mirror::InfoForType<void>() {
    static Mirror::TypeInfo localStaticTypeInfo; if (!localStaticTypeInfo.stringName.empty()) {
        return &localStaticTypeInfo;
    }
    localStaticTypeInfo.category = GetCategory<void>();
    localStaticTypeInfo.id = Mirror::TypeId<void>(); localStaticTypeInfo.stringName = typeid(void).name();
    SetConstructionLambda<void>(&localStaticTypeInfo, std::is_same<void, std::string>::type());
    return &localStaticTypeInfo;
}
// MIRROR_INFO_FOR_TYPE(void)
MIRROR_INFO_FOR_TYPE(void*)

// Mutable
MIRROR_INFO_FOR_TYPE(signed char)
MIRROR_INFO_FOR_TYPE(signed short)
MIRROR_INFO_FOR_TYPE(signed int)
MIRROR_INFO_FOR_TYPE(signed long long)
MIRROR_INFO_FOR_TYPE(unsigned char)
MIRROR_INFO_FOR_TYPE(unsigned short)
MIRROR_INFO_FOR_TYPE(unsigned int)
MIRROR_INFO_FOR_TYPE(unsigned long long)
MIRROR_INFO_FOR_TYPE(float)
MIRROR_INFO_FOR_TYPE(double)
MIRROR_INFO_FOR_TYPE(long double)
MIRROR_INFO_FOR_TYPE(char)
MIRROR_INFO_FOR_TYPE(bool)

// Const
MIRROR_INFO_FOR_TYPE(const signed char)
MIRROR_INFO_FOR_TYPE(const signed short)
MIRROR_INFO_FOR_TYPE(const signed int)
MIRROR_INFO_FOR_TYPE(const signed long long)
MIRROR_INFO_FOR_TYPE(const unsigned char)
MIRROR_INFO_FOR_TYPE(const unsigned short)
MIRROR_INFO_FOR_TYPE(const unsigned int)
MIRROR_INFO_FOR_TYPE(const unsigned long long)
MIRROR_INFO_FOR_TYPE(const float)
MIRROR_INFO_FOR_TYPE(const double)
MIRROR_INFO_FOR_TYPE(const long double)
MIRROR_INFO_FOR_TYPE(const char)
MIRROR_INFO_FOR_TYPE(const bool)

// Mutable pointers
MIRROR_INFO_FOR_TYPE(signed char*)
MIRROR_INFO_FOR_TYPE(signed short*)
MIRROR_INFO_FOR_TYPE(signed int*)
MIRROR_INFO_FOR_TYPE(signed long long*)
MIRROR_INFO_FOR_TYPE(unsigned char*)
MIRROR_INFO_FOR_TYPE(unsigned short*)
MIRROR_INFO_FOR_TYPE(unsigned int*)
MIRROR_INFO_FOR_TYPE(unsigned long long*)
MIRROR_INFO_FOR_TYPE(float*)
MIRROR_INFO_FOR_TYPE(double*)
MIRROR_INFO_FOR_TYPE(long double*)
MIRROR_INFO_FOR_TYPE(char*)
MIRROR_INFO_FOR_TYPE(bool*)

// Const pointers
MIRROR_INFO_FOR_TYPE(const signed char*)
MIRROR_INFO_FOR_TYPE(const signed short*)
MIRROR_INFO_FOR_TYPE(const signed int*)
MIRROR_INFO_FOR_TYPE(const signed long long*)
MIRROR_INFO_FOR_TYPE(const unsigned char*)
MIRROR_INFO_FOR_TYPE(const unsigned short*)
MIRROR_INFO_FOR_TYPE(const unsigned int*)
MIRROR_INFO_FOR_TYPE(const unsigned long long*)
MIRROR_INFO_FOR_TYPE(const float*)
MIRROR_INFO_FOR_TYPE(const double*)
MIRROR_INFO_FOR_TYPE(const long double*)
MIRROR_INFO_FOR_TYPE(const char*)
MIRROR_INFO_FOR_TYPE(const bool*)

// String
#include <string>
// #TODO Look to forward declare std::string instead of including header file
// namespace std { class basic_string; using string = basic_string<char, char_traits<char>, allocator<char>>; }
MIRROR_INFO_FOR_TYPE(std::string)
MIRROR_INFO_FOR_TYPE(std::string*)
MIRROR_INFO_FOR_TYPE(const std::string)
MIRROR_INFO_FOR_TYPE(const std::string*)
