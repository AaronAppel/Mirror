#include "MIR_Mirror.h"

#include <typeinfo>

#include "Main.h"
#include "ForMirror.h"

// MIRROR_TYPE(int[10])

// MIRROR_CLASS(UserCollection)
// MIRROR_CLASS_MEMBER(buffer)
// MIRROR_CLASS_END

// #TODO Create example user type reflection
#define MIRROR_USER_COLLECTION(USER_COLLECTION_TYPE, ELEMENT_TYPE) // #TODO Implement
template <> static const Mirror::TypeInfo* Mirror::InfoForType<UserCollection>() {
	static Mirror::TypeInfo localStaticTypeInfo;
	if (!localStaticTypeInfo.stringName.empty()) {
		return &localStaticTypeInfo;
	}
	localStaticTypeInfo.category = Mirror::TypeInfoCategory_Collection;
	localStaticTypeInfo.stringName = typeid(UserCollection).name();
	localStaticTypeInfo.id = Mirror::IdForType<UserCollection>();
	static_assert(sizeof(UserCollection) <= 0xffffui16, "Size is larger than member can hold!");
	localStaticTypeInfo.size = sizeof(UserCollection);

	localStaticTypeInfo.typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) UserCollection; };
	localStaticTypeInfo.collectionTypeInfoFirst = Mirror::InfoForType<int>();
	localStaticTypeInfo.collectionAddFunc = [](void* collectionAddress, size_t index, const void* elementFirst, const void* /*elementSecond*/) {
		memcpy((char*)collectionAddress + (sizeof(int) * index), elementFirst, sizeof(int));
		};
	localStaticTypeInfo.collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0;
		if (aIndex < index) index = aIndex;
		if (index >= sizeof(UserCollection) / sizeof(int)) { index = 0; return nullptr; }
		return (char*)collectionAddress + (sizeof(int) * index++);
		};

	return &localStaticTypeInfo;
}

