#pragma once

#include <string>
#include <vector>

#define MIRROR_MEMBER_FIELDS_DEFAULT 3
#define MIRROR_PRIVATE_MEMBERS friend struct Mirror;

#define MIRROR_FIELD_FLAG_SIZE uint8_t
#define MIRROR_TYPE_INFO_CATEGORY_SIZE uint8_t
#define MIRROR_TYPE_SIZE uint16_t
#define MIRROR_FIELD_ID_SIZE std::size_t
#define MIRROR_TYPE_SIZE_MAX UINT16_MAX

#ifdef MIRROR_TESTING
#include <cassert>
#define MIRROR_ASSERT(x) assert(x)
#else
#define MIRROR_ASSERT(x)
#endif

struct Mirror
{
	struct TypeInfo;

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

#ifndef MIRROR_TYPE_SIZE_UNUSED
		MIRROR_TYPE_SIZE size = 0;
#endif

#ifndef MIRROR_FIELD_FLAGS_UNUSED
		MIRROR_FIELD_FLAG_SIZE serializationFlags = 0;
#endif
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

#ifndef MIRROR_TYPE_SIZE_UNUSED
		MIRROR_TYPE_SIZE size = 0;
#endif
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { }; // #TODO Hide/private non-constants
		std::vector<const TypeInfo*> derivedTypes; // #TODO Utilize memory for non-derived types

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

		const TypeInfo* collectionTypeInfoFirst = nullptr; // #TODO Utilize memory for non-collections
		const TypeInfo* collectionTypeInfoSecond = nullptr;
		const TypeInfo* superTypeInfo = nullptr;
		const TypeInfo* pointerDereferencedTypeInfo = nullptr;

		Func_void_voidPtr_sizet_voidPtr_voidPtr collectionAddFunc = nullptr;
		Func_voidPtr_constVoidPtr_bool collectionAddressOfPairObjectFunc = nullptr;
		Func_void_voidPtr collectionClearFunction = nullptr;
		Func_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr;

		Func_void_voidPtr typeConstructorFunc = nullptr; // #TODO Utilize memory for non-classes
		Func_bool_constVoidPtr typeDynamicCastFunc = nullptr; // #TODO Utilize memory for non-classes
	};

	template <class T>
	static const TypeInfo* InfoForType(const T& typeObj);

	template<typename T>
	static const TypeInfo* InfoForType();

	template<typename... T>
	struct MirrorTemplateArgumentList { };

	template<typename T>
	static constexpr size_t TypeIdConstexpr() {
		static_assert(false && "Specialization of TypeIdConstexpr() missing!");
		return 0;
	}
};