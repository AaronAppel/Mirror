#pragma once

#include <array>
#include <string>
#include <vector>

#include "MirrorTypes.h"

#define MIRROR_TO_STR(x) #x

// Original inspiration for Mirror : https://github.com/danbar0/StaticJsonReflection/tree/master

// #TODO See if these macros could be helpful
// #define typeinfo(T) InfoForClass<T>()
// #define typeof(T) InfoForType<T>()->enumType

// #TODO Look at creating functions to do determine reflection data and write to a file,
// so no static functions are needed, and the data is only accessed and exists in RAM when needed.

#define MIRROR_MEMBER_FIELDS_DEFAULT 5
#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

struct Mirror
{
	struct TypeInfo {
		std::string stringName = "";
		MirrorTypes enumType = MirrorTypes::m_bool;
		size_t size = 0;
	};

	struct Field {
		const TypeInfo* type = nullptr;
		std::string name = "";
		size_t offset = 0;
	};

	struct ClassInfo {
		std::vector<Field> fields = { };
	};

	template<typename T>
	static const TypeInfo* InfoForType();

	template <class T>
	static const ClassInfo* InfoForClass();

	template <class T>
	static const ClassInfo* InfoForClass(const T& classObj);

	// #NOTE GetClassInfo taken by WinUser.h
};

template <class TYPE>
static const Mirror::ClassInfo* Mirror::InfoForClass(const TYPE& classObj)
{
return Mirror::InfoForClass<TYPE>();
}

// For primitive types (int8_t, char, const char*, etc)
#define MIRROR_TYPE(TYPE) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.enumType = MirrorTypes::TYPE; \
	return &localStaticTypeInfo; \
}\

// #NOTE Duplicated above MIRROR_TYPE in MIRROR_CLASS_START to avoid need for using both defines
// #NOTE The using carries the type name from the MIRROR_CLASS into the MIRROR_CLASS_MEMBER so it isn't needed
// as another argument
#define MIRROR_CLASS_START(TYPE) MIRROR_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_CLASS_STARTN(TYPE, COUNT) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.enumType = MirrorTypes::TYPE; \
	return &localStaticTypeInfo; \
} \
 \
template <> \
const Mirror::ClassInfo* Mirror::InfoForClass<TYPE>() \
{ \
	static Mirror::ClassInfo localStaticClassInfo; \
	const int fieldsCount = COUNT; \
	localStaticClassInfo.fields.reserve(fieldsCount); \
 \
	using ClassType = TYPE; \
	enum { BASE = __COUNTER__ }; \

#define MIRROR_CLASS_MEMBER(MEMBER_NAME) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1}; \
	Mirror::Field MEMBER_NAME##field; \
	localStaticClassInfo.fields.push_back(MEMBER_NAME##field); \
	localStaticClassInfo.fields[MEMBER_NAME##Index].type = Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>(); \
	localStaticClassInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME; \
	localStaticClassInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME); \

#define MIRROR_CLASS_END \
	if (localStaticClassInfo.fields.size() >= fieldsCount) { bool warning = true; } \
	return &localStaticClassInfo; \
} \
