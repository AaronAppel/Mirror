#pragma once

#include <string>
#include <vector>

// #NOTE Functionality macros. See Macros section in README.md for more info.
// #define MIRROR_DEBUG

#ifdef MIRROR_DEBUG
#include <cassert>
#define MIRROR_ASSERT(x) assert(x)
#else
#define MIRROR_ASSERT(x)
#endif

#define MIRROR_PRIVATE_MEMBERS friend struct Mir;

#define MIRROR_FIELD_FLAG_TYPE uint8_t
#define MIRROR_FIELD_FLAG_MAX UINT8_MAX
#define MIRROR_TYPE_ID_TYPE uint8_t
#define MIRROR_TYPE_ID_MAX UINT8_MAX
#define MIRROR_TYPE_SIZE_TYPE uint16_t
#define MIRROR_TYPE_SIZE_MAX UINT16_MAX
#define MIRROR_TYPE_CATEGORY_TYPE uint8_t
#define MIRROR_TYPE_CATEGORY_SIZE_MAX UINT8_MAX

struct Mir
{

	enum TypeInfoCategories : uint8_t
	{
		TypeInfoCategory_Primitive = 0,
		TypeInfoCategory_Class,
		TypeInfoCategory_Collection,
		TypeInfoCategory_Pair, // #TODO Remove pair as in QwerkE latest Mirror implementation
		TypeInfoCategory_Pointer,
	};

	struct TypeInfo;

	struct Field
	{
		const TypeInfo* typeInfo = nullptr;

		std::string name = "";
		std::size_t offset = 0;
		MIRROR_TYPE_SIZE_TYPE size = 0;
		MIRROR_FIELD_FLAG_TYPE flags = 0;
	};

	struct TypeInfo
	{
		std::string stringName = "";
		MIRROR_TYPE_ID_TYPE id = 0;

#ifndef MIRROR_TYPE_SIZE_UNUSED
		MIRROR_TYPE_SIZE_TYPE size = 0;
#endif
		TypeInfoCategories category = TypeInfoCategories::TypeInfoCategory_Primitive;

		std::vector<Field> fields = { };
		std::vector<const TypeInfo*> derivedTypes;

		const TypeInfo* collectionTypeInfoFirst = nullptr;
		const TypeInfo* collectionTypeInfoSecond = nullptr;
		// #TODO Support tuples and handle collection logic type-agnosticly using a vector of const TypeInfo*s?
		const TypeInfo* superTypeInfo;
		const TypeInfo* pointerDereferencedTypeInfo;

		using FuncPtr_void_voidPtr_sizet_constvoidPtr_constvoidPtr = void (*)(void*, size_t, const void*, const void*);
		using FuncPtr_void_voidPtr = void (*)(void*);
		using FuncPtr_voidPtr_constVoidPtr_bool = void* (*)(const void*, bool);
		using FuncPtr_charPtr_constVoidPtr_sizet = char* (*)(const void*, size_t);
		using FuncPtr_bool_constVoidPtr = bool (*)(const void*);

		FuncPtr_void_voidPtr_sizet_constvoidPtr_constvoidPtr collectionAddFunc = nullptr;
		FuncPtr_voidPtr_constVoidPtr_bool collectionAddressOfPairObjectFunc = nullptr;
		FuncPtr_void_voidPtr collectionClearFunction = nullptr;
		FuncPtr_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr;

		FuncPtr_void_voidPtr typeConstructorFunc = nullptr;
		FuncPtr_bool_constVoidPtr typeDynamicCastFunc = nullptr;

		const TypeInfo* AbsoluteType() const {
			return pointerDereferencedTypeInfo ? pointerDereferencedTypeInfo : this;
		}

		const TypeInfo* DerivedTypeFromPointer(const void* instanceAddress) const {
			if (derivedTypes.empty()) return this;
			for (const auto& derivedType : derivedTypes) {
				MIRROR_ASSERT(derivedType->typeDynamicCastFunc && "Null typeDynamicCastFunc!");
				if (derivedType->typeDynamicCastFunc(instanceAddress)) {
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
			if (collectionAddFunc) {
				collectionAddFunc(collectionAddress, index, first, second);
			}
		}

		void Construct(void* instanceAddress) const {
			MIRROR_ASSERT(collectionAddFunc && "Null typeConstructorFunc!");
			if (typeConstructorFunc) {
				typeConstructorFunc(instanceAddress);
			}
		}

		void ClearCollection(void* collectionAddress) const {
			MIRROR_ASSERT(collectionAddFunc && "Null collectionClearFunction!");
			if (collectionClearFunction) {
				collectionClearFunction(collectionAddress);
			}
		}
	};

	template <typename T>
	static const TypeInfo* Info(const T& typeObj);

	template <typename T>
	static const TypeInfo* Info();

	template <typename T>
	static constexpr MIRROR_TYPE_ID_TYPE Id(const T& typeObj);

	template <typename T>
	static constexpr MIRROR_TYPE_ID_TYPE Id();

#define MIRROR_TYPE_ID_IMPL(ID, TYPE) \
	template <> constexpr MIRROR_TYPE_ID_TYPE Mir::Id<TYPE>() { return ID; }

#define MIRROR_TYPE_ID(ID, ...) MIRROR_TYPE_ID_IMPL(ID, __VA_ARGS__)

	template<typename... T>
	struct TypesList {};

};
