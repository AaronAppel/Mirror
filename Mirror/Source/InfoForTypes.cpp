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
// MIRROR_INFO_TYPE(void)
template<> static const Mirror::TypeInfo* Mirror::InfoForType<void*>() {
    static_assert(sizeof(void*) <= 0xffffui16, "Size is larger than member can hold!"); static Mirror::TypeInfo localStaticTypeInfo; if (!localStaticTypeInfo.stringName.empty()) {
        return &localStaticTypeInfo;
    } localStaticTypeInfo.category = GetCategory<void*>(); localStaticTypeInfo.stringName = "void*"; localStaticTypeInfo.id = Mirror::TypeId<void*>(); localStaticTypeInfo.size = sizeof(void*); switch (localStaticTypeInfo.category) {
    case TypeInfoCategory_Collection: case TypeInfoCategory_Pair: SetCollectionLambdas<void*>(&localStaticTypeInfo, is_stl_container_impl::is_stl_container<void*>::type()); case TypeInfoCategory_Class: SetConstructionLambda<void*>(&localStaticTypeInfo, std::is_class<void*>::type()); break; case TypeInfoCategory_Pointer: localStaticTypeInfo.pointerDereferencedTypeInfo = Mirror::InfoForType<std::remove_pointer_t<void*>>(); break; case TypeInfoCategory_Primitive: SetConstructionLambda<void*>(&localStaticTypeInfo, std::is_same<void*, std::string>::type()); break;
    } return &localStaticTypeInfo;
}

// Mutable
MIRROR_INFO_TYPE(signed char)
MIRROR_INFO_TYPE(signed short)
MIRROR_INFO_TYPE(signed int)
MIRROR_INFO_TYPE(signed long long)
MIRROR_INFO_TYPE(unsigned char)
MIRROR_INFO_TYPE(unsigned short)
MIRROR_INFO_TYPE(unsigned int)
MIRROR_INFO_TYPE(unsigned long long)
MIRROR_INFO_TYPE(float)
MIRROR_INFO_TYPE(double)
MIRROR_INFO_TYPE(long double)
MIRROR_INFO_TYPE(char)
MIRROR_INFO_TYPE(bool)

// Const
MIRROR_INFO_TYPE(const signed char)
MIRROR_INFO_TYPE(const signed short)
MIRROR_INFO_TYPE(const signed int)
MIRROR_INFO_TYPE(const signed long long)
MIRROR_INFO_TYPE(const unsigned char)
MIRROR_INFO_TYPE(const unsigned short)
MIRROR_INFO_TYPE(const unsigned int)
MIRROR_INFO_TYPE(const unsigned long long)
MIRROR_INFO_TYPE(const float)
MIRROR_INFO_TYPE(const double)
MIRROR_INFO_TYPE(const long double)
MIRROR_INFO_TYPE(const char)
MIRROR_INFO_TYPE(const bool)

// Mutable pointers
MIRROR_INFO_TYPE(signed char*)
MIRROR_INFO_TYPE(signed short*)
MIRROR_INFO_TYPE(signed int*)
MIRROR_INFO_TYPE(signed long long*)
MIRROR_INFO_TYPE(unsigned char*)
MIRROR_INFO_TYPE(unsigned short*)
MIRROR_INFO_TYPE(unsigned int*)
MIRROR_INFO_TYPE(unsigned long long*)
MIRROR_INFO_TYPE(float*)
MIRROR_INFO_TYPE(double*)
MIRROR_INFO_TYPE(long double*)
MIRROR_INFO_TYPE(char*)
MIRROR_INFO_TYPE(bool*)

// Const pointers
MIRROR_INFO_TYPE(const signed char*)
MIRROR_INFO_TYPE(const signed short*)
MIRROR_INFO_TYPE(const signed int*)
MIRROR_INFO_TYPE(const signed long long*)
MIRROR_INFO_TYPE(const unsigned char*)
MIRROR_INFO_TYPE(const unsigned short*)
MIRROR_INFO_TYPE(const unsigned int*)
MIRROR_INFO_TYPE(const unsigned long long*)
MIRROR_INFO_TYPE(const float*)
MIRROR_INFO_TYPE(const double*)
MIRROR_INFO_TYPE(const long double*)
MIRROR_INFO_TYPE(const char*)
MIRROR_INFO_TYPE(const bool*)

// String
#include <string>
// #TODO Look to forward declare std::string instead of including header file
// namespace std { class basic_string; using string = basic_string<char, char_traits<char>, allocator<char>>; }
MIRROR_INFO_TYPE(std::string)
MIRROR_INFO_TYPE(std::string*)
MIRROR_INFO_TYPE(const std::string)
MIRROR_INFO_TYPE(const std::string*)
