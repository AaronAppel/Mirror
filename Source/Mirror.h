#pragma once

#include <string>
#include <array>

#include "MirrorTypes.h"

#define MIRROR_TO_STR(x) #x

// Original inspiration for Mirror : https://github.com/danbar0/StaticJsonReflection/tree/master

// #TODO See if these macros could be helpful
// #define typeinfo(T) InfoForClass<T>()
// #define typeof(T) InfoForType<T>()->enumType

#define MIRROR_MAX_MEMBER_FIELDS 10

namespace Mirror
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
		std::array<Field, MIRROR_MAX_MEMBER_FIELDS> fields;
	};

	template<typename T>
	const TypeInfo* InfoForType();

	template<typename T>
	const ClassInfo* InfoForClass(); // #NOTE GetClassInfo taken by WinUser.h
}

#define MIRROR_TYPE(TYPE) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static TypeInfo type; \
	type.stringName = #TYPE; \
	type.size = sizeof(TYPE); \
	type.enumType = MirrorTypes::TYPE; \
	return &type; \
}\

#define MIRROR_MEMBERS_FOR(CLASS)  \
template<> \
const Mirror::ClassInfo* Mirror::InfoForClass<CLASS>() { \
	using ClassType = CLASS; \
	static ClassInfo localClass; \
	enum { BASE = __COUNTER__ }; \

#define MIRROR_MEMBER(MEMBER_NAME)  \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1}; \
	localClass.fields[MEMBER_NAME##Index].type = Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>();\
	localClass.fields[MEMBER_NAME##Index].name = { #MEMBER_NAME };  \
	localClass.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);\

#define END_MEMBER_MIRROR \
return &localClass; \
}\
