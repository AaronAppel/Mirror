#pragma once

#include <string>
#include <vector>

#include <typeinfo>
// #include <type_traits>

#include "TypeDeduction.h"
#include "ConstexprCounter.h"

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

// #TODO Configure build system to enable/disable testing asserts
#ifndef MIRROR_TESTING
#define MIRROR_TESTING
#endif

#ifdef MIRROR_TESTING
#include <cassert>
#define MIRROR_ASSERT(x) assert(x)
#else
#define MIRROR_ASSERT(x)
#endif

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

	using Func_void_voidPtr_sizet_voidPtr_voidPtr = void (*)(void*, size_t, void*, void*);
	using Func_void_voidPtr = void (*)(void*);
	using Func_voidPtr_constVoidPtr_bool = void* (*)(const void*, bool);
	using Func_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);
	using Func_bool_constVoidPtr = bool (*)(const void*);

	// struct PointersUnion
	// {
	// 	const TypeInfo* collectionTypeInfoFirst = nullptr;
	// 	const TypeInfo* superTypeInfo = nullptr;
	// 	const TypeInfo* pointerDereferencedTypeInfo = nullptr;
	// };

	struct TypeInfo
	{
		std::string stringName = "";
		MIRROR_FIELD_ID_SIZE id = 0;
		MIRROR_TYPE_SIZE size = 0;
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { }; // #TODO Hide/private non-constants
		std::vector<const TypeInfo*> derivedTypes;

		const TypeInfo* AbsoluteType() const {
			return pointerDereferencedTypeInfo ? pointerDereferencedTypeInfo : this;
		}

		const TypeInfo* DerivedTypeFromPointer(const void* instanceAddress) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes)
			{
				MIRROR_ASSERT(derivedType->typeDynamicCastFunc && "Null typeDynamicCastFunc!");
				if (derivedType->typeDynamicCastFunc(instanceAddress))
				{
					return derivedType;
				}
			}
			return this;
		}

		const TypeInfo* DerivedTypeFromTypeName(const std::string& typeName) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes)
			{
				if (strcmp(derivedType->stringName.c_str(), typeName.c_str()) == 0)
				{
					return derivedType;
				}
			}
			return this;
		}

		void CollectionAppend(void* collectionAddress, size_t index, void* first, void* second = nullptr) const {
			MIRROR_ASSERT(collectionAddFunc && "Null collectionAddFunc!");
			if (collectionAddFunc)
			{
				collectionAddFunc(collectionAddress, index, first, second);
			}
		}

		void Construct(void* instanceAddress) const {
			MIRROR_ASSERT(collectionAddFunc && "Null typeConstructorFunc!");
			if (typeConstructorFunc)
			{
				typeConstructorFunc(instanceAddress);
			}
		}

		void ClearCollection(void* collectionAddress) const {
			MIRROR_ASSERT(collectionAddFunc && "Null collectionClearFunction!");
			if (collectionClearFunction)
			{
				collectionClearFunction(collectionAddress);
			}
		}

		// #TODO Unions : collectionTypeInfoFirst : superTypeInfo : pointerDereferencedTypeInfo,
		// const TypeInfo* pointers[2]; Size of collection info types
		// PointersUnion pointers;

		const TypeInfo* collectionTypeInfoFirst = nullptr;
		const TypeInfo* collectionTypeInfoSecond = nullptr;
		const TypeInfo* superTypeInfo = nullptr;
		const TypeInfo* pointerDereferencedTypeInfo = nullptr;
		// #TODO Consider adding a reference to the underlying type (uint8_t is typedef'd from unsigned char)
		// const TypeInfo* underlyingType = nullptr;

		Func_void_voidPtr_sizet_voidPtr_voidPtr collectionAddFunc = nullptr;
		Func_voidPtr_constVoidPtr_bool collectionFirstSecondFunc = nullptr; // #TODO Better name. It returns the address of 1st if arg true, or 2nd if false
		Func_void_voidPtr collectionClearFunction;
		Func_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr;

		Func_void_voidPtr typeConstructorFunc = nullptr;
		Func_bool_constVoidPtr typeDynamicCastFunc = nullptr;
	};

	template <class T>
	static const TypeInfo* InfoForType(const T& typeObj);

	template<typename T>
	static const TypeInfo* InfoForType();

	template<typename... T>
	struct MirrorTemplateArgumentList { };

	template<typename T>
	static size_t TypeId();

	// template<typename T>
	// static constexpr size_t TypeId()
	// {
	// 	return next();
	// }
};

#define TYPE_ID(TYPE) \
template <> \
size_t Mirror::TypeId<TYPE>() \
{ \
	return __COUNTER__; \
}

template <typename T>
constexpr Mirror::TypeInfoCategories GetCategory()
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
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::true_type)
{
	static_assert(std::is_class_v<T> || std::is_same_v<T, std::string>);
	// #TODO Look into std::remove_const_t<T>;
	Mirror::TypeInfo* typeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	typeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
}

template<typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::true_type)
{
	static_assert(is_stl_vector_impl::is_stl_vector<T>::type());
	Mirror::TypeInfo* typeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	typeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>(); // #TODO Same as Map

	typeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, void* elementFirst, void* /*elementSecond*/) {
		((T*)collectionAddress)->push_back(*(T::value_type*)elementFirst); // #TODO Similar to Map
	};
	typeInfo->collectionClearFunction = [](void* collectionAddress) {
		((T*)collectionAddress)->clear(); // #TODO Similar to Map
	};
	typeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0;
		T* vector = ((T*)collectionAddress);
		if (aIndex < index) index = aIndex;
		if (index >= vector->size()) { index = 0; return nullptr; }
		return (char*)vector->data() + (sizeof(T::value_type) * index++);
	};
}

template<typename T>
static void SetCollectionLambdasMap(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetCollectionLambdasMap(Mirror::TypeInfo* constTypeInfo, std::true_type)
{
	static_assert(is_stl_container<T>::value);
	static_assert(is_stl_map<T>::value);

	Mirror::TypeInfo* typeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	typeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>(); // #TODO Same as Vector

	typeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, void* elementFirst, void* /*elementSecond*/) {
		((T*)collectionAddress)->insert(*(T::value_type*)elementFirst); // #TODO Similar to Vector
	};
	typeInfo->collectionClearFunction = [](void* collectionAddress) {
		((T*)collectionAddress)->clear(); // #TODO Similar to Vector
	};
	typeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0;
		T* map = (T*)collectionAddress;
		static T::iterator iterator = map->begin();
		if (aIndex < index || map->end() == iterator)
		{
			index = aIndex;
			iterator = map->begin();
		}
		if (index >= map->size()) { ++index; return nullptr; }
		++index;
		auto result = (char*)&iterator->first;
		++iterator;
		return result;
	};
}

template<typename T>
static void SetCollectionLambdasPair(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template<typename T>
static void SetCollectionLambdasPair(Mirror::TypeInfo* constTypeInfo, std::true_type)
{
	static_assert(is_stl_pair<T>::value);
	// #TODO static_assert(is_stl_vector_impl::is_stl_vector<T>::type());
	Mirror::TypeInfo* typeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	typeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::first_type>();
	typeInfo->collectionTypeInfoSecond = Mirror::InfoForType<T::second_type>();

	typeInfo->collectionAddFunc = [](void* pairObjAddress, size_t /*index*/, void* elementFirst, void* elementSecond) {
		T* pair = (T*)pairObjAddress;

		const Mirror::TypeInfo* firstTypeInfo = Mirror::InfoForType<T::first_type>();
		firstTypeInfo->Construct((void*)&pair->first);

		const Mirror::TypeInfo* secondTypeInfo = Mirror::InfoForType<T::second_type>();
		secondTypeInfo->Construct((void*)&pair->second);
	};
	typeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
	typeInfo->collectionFirstSecondFunc = [](const void* pairObjAddress, bool isFirst) -> void* {
		T* pair = (T*)pairObjAddress;
		if (isFirst) return (void*)&pair->first;
		return (void*)&pair->second;
	};
}

template<typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo, std::false_type)
{
	Mirror::TypeInfo* typeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	if (std::is_array_v<T>)
	{
		typedef typename std::remove_all_extents<T>::type ArrayElementType;
		typeInfo->collectionTypeInfoFirst = Mirror::InfoForType<ArrayElementType>();
		typeInfo->collectionAddFunc = [](void* collectionAddress, size_t index, void* elementFirst, void* /*elementSecond*/) {
			memcpy((char*)collectionAddress + (sizeof(ArrayElementType) * index), elementFirst, sizeof(ArrayElementType));
		};
		typeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
			static size_t index = 0;
			if (aIndex < index) index = aIndex;
			if (index >= sizeof(ArrayElementType) / sizeof(ArrayElementType)) { index = 0; return nullptr; }
			return (char*)collectionAddress + (sizeof(ArrayElementType) * index++);
		};
	}
	else if (is_stl_pair<T>::value)
	{
		SetCollectionLambdasPair<T>(typeInfo, is_stl_pair_impl::is_stl_pair<T>::type());
	}
}

template<typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo, std::true_type)
{
	static_assert(is_stl_container<T>::value);
	// #TODO Look into std::remove_const_t<T>;

	Mirror::TypeInfo* typeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	typedef typename std::remove_all_extents<T>::type CollectionElementTypeFirst;
	typeInfo->collectionTypeInfoFirst = Mirror::InfoForType<CollectionElementTypeFirst>();

	// #TODO Look at how to support all types without manual implementations
	SetCollectionLambdasVector<T>(typeInfo, is_stl_vector_impl::is_stl_vector<T>::type());
	SetCollectionLambdasMap<T>(typeInfo, is_stl_map_impl::is_stl_map<T>::type());
}

template <typename T>
static size_t Mirror::TypeId()
{
	return typeid(T).hash_code();
}

template<typename T>
static const Mirror::TypeInfo* Mirror::InfoForType()
{
	// #NOTE remove_all_extents for the case of arrays
	static_assert(sizeof(T) <= MIRROR_TYPE_SIZE_MAX, "Size is larger than member can hold!");
	static Mirror::TypeInfo localStaticTypeInfo;

	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; }
	localStaticTypeInfo.category = GetCategory<T>();

	// #TODO Format type name string
	localStaticTypeInfo.stringName = typeid(T).name();
	size_t index = localStaticTypeInfo.stringName.find_last_of(':');
	if (index != localStaticTypeInfo.stringName.npos)
	{
		// localStaticTypeInfo.stringName = localStaticTypeInfo.stringName.substr(index + 1); // "class NameSpace::Mesh" -> "Mesh"
	}
	// localStaticTypeInfo.id = HashFromString<T>(localStaticTypeInfo.stringName.c_str());

	// #TODO Fix type Id linker errors
	localStaticTypeInfo.id = Mirror::TypeId<T>();

	// #NOTE remove_all_extents for the case of arrays
	// #TODO Review size for arrays is correct
	localStaticTypeInfo.size = sizeof(T);

	switch (localStaticTypeInfo.category)
	{
	case TypeInfoCategory_Collection:
	case TypeInfoCategory_Pair:
		SetCollectionLambdas<T>(&localStaticTypeInfo, is_stl_container_impl::is_stl_container<T>::type());
	case TypeInfoCategory_Class:
		SetConstructionLambda<T>(&localStaticTypeInfo, std::is_class<T>::type());
		break;

	case TypeInfoCategory_Pointer:
		// static_assert(std::is_pointer_v<TYPE>);
		localStaticTypeInfo.pointerDereferencedTypeInfo = Mirror::InfoForType<std::remove_pointer_t<T>>();
		break;

	case TypeInfoCategory_Primitive:
		SetConstructionLambda<T>(&localStaticTypeInfo, std::is_same<T, std::string>::type());
		break;
	}

	return &localStaticTypeInfo;
}

//  localStaticTypeInfo.id = TypeId<TYPE>();
#define MIRROR_TYPE(TYPE) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.category = GetCategory<TYPE>();	\
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	return &localStaticTypeInfo; \
}

//  localStaticTypeInfo.id = TypeId<TYPE>();
#define MIRROR_MAP(COLLECTION_TYPE, FIRST_TYPE, SECOND_TYPE) \
template <> \
const Mirror::TypeInfo* Mirror::InfoForType<COLLECTION_TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.category = GetCategory<COLLECTION_TYPE>();	\
	localStaticTypeInfo.stringName = #COLLECTION_TYPE; \
	localStaticTypeInfo.size = sizeof(COLLECTION_TYPE); \
	localStaticTypeInfo.collectionClearFunction = [](void* collectionAddress) { ((COLLECTION_TYPE*)collectionAddress)->clear(); }; \
	return &localStaticTypeInfo; \
}

//  localStaticTypeInfo.id = TypeId<TYPE>();
#define MIRROR_CLASS_START(TYPE) MIRROR_CLASS_STARTN(TYPE, MIRROR_MEMBER_FIELDS_DEFAULT)
#define MIRROR_CLASS_STARTN(TYPE, FIELDCOUNT) \
TYPE_ID(TYPE) \
 \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static Mirror::TypeInfo localStaticTypeInfo; \
	if (!localStaticTypeInfo.stringName.empty()) { return &localStaticTypeInfo; } \
	localStaticTypeInfo.category = GetCategory<TYPE>();	\
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

// #TODO Produces wrong ids: Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>()
#define MIRROR_CLASS_MEMBER(MEMBER_NAME) \
	enum { MEMBER_NAME##Index = __COUNTER__ - BASE - 1 }; \
	Mirror::Field MEMBER_NAME##field; \
	localStaticTypeInfo.fields.push_back(MEMBER_NAME##field); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].typeInfo = Mirror::InfoForType<decltype(ClassType::MEMBER_NAME)>(); \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].name = #MEMBER_NAME; \
	localStaticTypeInfo.fields[MEMBER_NAME##Index].offset = offsetof(ClassType, MEMBER_NAME);

#define MIRROR_CLASS_END \
	return &localStaticTypeInfo; \
}

//  localStaticTypeInfo.id = TypeId<TYPE>();
// #NOTE Experimental collection type macro (currently unused)
#define MIRROR_COLLECTION(TYPE, COLLECTIONTYPE) \
template<> \
const Mirror::TypeInfo* Mirror::InfoForType<TYPE>() { \
	static TypeInfo localStaticTypeInfo; \
	localStaticTypeInfo.stringName = #TYPE; \
	localStaticTypeInfo.size = sizeof(TYPE); \
	localStaticTypeInfo.category = GetCategory<TYPE>();	\
	localStaticTypeInfo.collectionTypeInfo = Mirror::InfoForType<COLLECTIONTYPE>(); \
	return &localStaticTypeInfo; \
}
