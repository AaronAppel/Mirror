#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "MirrorUtils.h"
#include "TypeDeduction.h"

#define MIRROR_TO_STR(x) #x

// Original inspiration for Mirror : https://github.com/danbar0/StaticJsonReflection/tree/master

// #TODO Fix indentation issues from macros

#define MIRROR_MEMBER_FIELDS_DEFAULT 3
#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

#define MIRROR_FIELD_FLAG_SIZE uint8_t
#define MIRROR_TYPE_INFO_CATEGORY_SIZE uint8_t
#define MIRROR_TYPE_SIZE uint16_t
#define MIRROR_FIELD_ID_SIZE std::size_t
#define MIRROR_TYPE_SIZE_MAX UINT16_MAX

#define MIRROR_TYPE_ID(x) HashFromString(#x)

struct Mirror
{
	struct TypeInfo;

	enum FieldSerializationFlags : MIRROR_FIELD_FLAG_SIZE
	{
		_None = 0,
		// Your flags here
	};

	enum TypeInfoCategories : MIRROR_TYPE_INFO_CATEGORY_SIZE
	{
		TypeInfoCategory_Primitive = 0,
		TypeInfoCategory_Class,
		TypeInfoCategory_Collection,
		TypeInfoCategory_Pair,
		TypeInfoCategory_Pointer,
	};

	struct Field
	{
		const TypeInfo* typeInfo = nullptr;
		std::string name = "";
		std::size_t offset = 0;
		MIRROR_TYPE_SIZE size = 0; // #TODO Review how valuable member size is, or where it's currently used

		// #TODO See if templated function can have default flag argument
		// #TODO Consider hiding field flag feature with #defines (#MIRROR_FIELD_FLAGS_UNUSED)
		MIRROR_FIELD_FLAG_SIZE serializationFlags = FieldSerializationFlags::_None;
	};

	using ClearFunction = void (*)(void*);

	struct TypeInfo
	{
		std::string stringName = "";
		MIRROR_FIELD_ID_SIZE id = 0;
		MIRROR_TYPE_SIZE size = 0;
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { }; // #TODO Hide/private non-constants
		std::vector<const TypeInfo*> derivedTypes;

		const TypeInfo* collectionTypeInfo = nullptr; // #TODO Union with superTypeInfo?
		const TypeInfo* superTypeInfo = nullptr;

		ClearFunction clearFunction;
		void ClearCollection(void* collectionAddress) const { clearFunction(collectionAddress); }
	};

	template <class T>
	static const TypeInfo* InfoForType(const T& typeObj);

	template<typename T>
	static const TypeInfo* InfoForType();

};

template <typename T>
constexpr Mirror::TypeInfoCategories SetCategory()
{
	// #TODO Review if possible special cases exist, and proper ordering
	if (std::is_same_v<T, std::string>) return Mirror::TypeInfoCategory_Primitive;
	if (std::is_same_v<T, const char*>) return Mirror::TypeInfoCategory_Primitive;
	if (std::is_enum_v<T>) return Mirror::TypeInfoCategory_Primitive; // #REVIEW Needed or useful?

	if (std::is_array_v<T>) return Mirror::TypeInfoCategory_Collection;
	if (std::is_pointer_v<T>) return Mirror::TypeInfoCategory_Pointer;
	if (is_stl_pair<T>::value) return Mirror::TypeInfoCategory_Pair;
	if (is_stl_container<T>::value) return Mirror::TypeInfoCategory_Collection;
	if (std::is_class_v<T>) return Mirror::TypeInfoCategory_Class;
	else return Mirror::TypeInfoCategory_Primitive;
}

template <class TYPE>
static const Mirror::TypeInfo* Mirror::InfoForType(const TYPE& typeObj)
{
	return Mirror::InfoForType<TYPE>();
}

template<typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo)
{
	Mirror::TypeInfo* typeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	// #TODO Add lambdas (construct, add to collection, iterate collection)
	// typeInfo->
}

template<typename T>
static const Mirror::TypeInfo* Mirror::InfoForType()
{
	static_assert(sizeof(T) <= MIRROR_TYPE_SIZE_MAX, "Size is larger than member can hold!");
	static Mirror::TypeInfo localStaticTypeInfo;

	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; }
	localStaticTypeInfo.id = HashFromString(localStaticTypeInfo.stringName.c_str());
	localStaticTypeInfo.category = SetCategory<T>();
	localStaticTypeInfo.stringName = typeid(T).name();
	size_t index = localStaticTypeInfo.stringName.find_last_of(':');
	if (index != localStaticTypeInfo.stringName.npos)
	{
		localStaticTypeInfo.stringName = localStaticTypeInfo.stringName.substr(index + 1); // "class QwerkE::Mesh" -> "Mesh"
	}
	localStaticTypeInfo.size = sizeof(T);

	if (TypeInfoCategory_Collection == localStaticTypeInfo.category)
	{
		SetCollectionLambdas<T>(&localStaticTypeInfo);
	}

	return &localStaticTypeInfo;
}

#define MIRROR_TYPE(TYPE) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.id = HashFromString(localStaticTypeInfo.stringName.c_str()); \
	localStaticTypeInfo.category = SetCategory<TYPE>();	\
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	return &localStaticTypeInfo; \
}

#define MIRROR_MAP(COLLECTION_TYPE, FIRST_TYPE, SECOND_TYPE) \
template <> \
const Mirror::TypeInfo* Mirror::InfoForType<COLLECTION_TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.id = HashFromString(localStaticTypeInfo.stringName.c_str()); \
	localStaticTypeInfo.category = SetCategory<COLLECTION_TYPE>();	\
	localStaticTypeInfo.stringName = #COLLECTION_TYPE; \
	localStaticTypeInfo.size = sizeof(COLLECTION_TYPE); \
	localStaticTypeInfo.clearFunction = [](void* collectionAddress) { ((COLLECTION_TYPE*)collectionAddress)->clear(); }; \
	return &localStaticTypeInfo; \
}

#define MIRROR_CLASS_START(TYPE) MIRROR_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_CLASS_STARTN(TYPE, FIELDCOUNT) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static Mirror::TypeInfo localStaticTypeInfo; \
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.id = HashFromString(localStaticTypeInfo.stringName.c_str()); \
	localStaticTypeInfo.category = SetCategory<TYPE>();	\
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
    if (localStaticTypeInfo.fields.size() > 0) { return &localStaticTypeInfo; } \
	const int fieldsCount = FIELDCOUNT; \
	localStaticTypeInfo.fields.reserve(fieldsCount); \
 \
	using ClassType = TYPE; \
	enum { BASE = __COUNTER__ };

#define MIRROR_CLASS_SUBCLASS(SUBCLASS_TYPE) \
	const Mirror::TypeInfo* SUBCLASS_TYPE##Info = Mirror::InfoForType<SUBCLASS_TYPE>(); \
	localStaticTypeInfo.derivedTypes.push_back(SUBCLASS_TYPE##Info); \
	const_cast<Mirror::TypeInfo*>(SUBCLASS_TYPE##Info)->superTypeInfo = &localStaticTypeInfo; \

#define MIRROR_CLASS_MEMBER(MEMBER_NAME) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 }; \
	Mirror::Field MEMBER_NAME##field; \
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].typeInfo = Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>(); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME; \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);

#define MIRROR_CLASS_END(TYPE) \
	if (localStaticTypeInfo.fields.size() >= fieldsCount) { bool warning = true; } \
	return &localStaticTypeInfo; \
} \
static const Mirror::TypeInfo* TYPE##typeInfo = Mirror::InfoForType<TYPE>(); \
// Call above initializes field class reference(s). Ideally, remove it and find another init method

// #NOTE Experimental collection type macro (currently unused)
#define MIRROR_COLLECTION(TYPE, COLLECTIONTYPE) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.id = HashFromString(localStaticTypeInfo.stringName.c_str()); \
	localStaticTypeInfo.category = SetCategory<TYPE>();	\
	localStaticTypeInfo.collectionTypeInfo = Mirror::InfoForType<COLLECTIONTYPE>(); \
	return &localStaticTypeInfo; \
}
