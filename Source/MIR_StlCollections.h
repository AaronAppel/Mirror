#pragma once

#include "MIR_Structs.h"
#include "MIR_TypeDeduction.h"
#include "MIR_Ids.h"

// #TODO multimap forced const key type
#define GENERIC_ITERATE_LAMBDA												\
	mutableTypeInfo->collectionIterateCurrentFunc =							\
		[](const void* collectionAddress, size_t aIndex) -> char* {			\
		T* collection = (T*)collectionAddress;                              \
		static auto iterator = collection->begin();                         \
		/* May also need to store reference to last collection */			\
		/* and reset when referencing a different collection */				\
		static size_t lastIndex = 0;										\
																			\
		/* if (aIndex >= collection->size()) */								\
		if (collection->end() == iterator)									\
		{																	\
			return nullptr;													\
		}																	\
																			\
		if (aIndex < lastIndex)												\
		{																	\
			iterator = collection->begin();									\
			for (size_t i = 0; i < aIndex; i++)                             \
			{																\
				++iterator;													\
				if (collection->end() == iterator)							\
				{															\
					return nullptr;											\
				}															\
			}																\
		}																	\
		else																\
		{																	\
			for (size_t i = 0; i < aIndex - lastIndex; i++)                 \
			{																\
				++iterator;													\
				if (collection->end() == iterator)							\
				{															\
					return nullptr;											\
				}															\
			}																\
		}																	\
		lastIndex = aIndex;													\
																			\
		if (collection->end() == iterator)									\
		{																	\
			return nullptr;													\
		}																	\
																			\
		return (char*)&(*iterator);											\
	};

template <typename T>
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetConstructionLambda(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(std::is_class_v<T> || std::is_same_v<T, std::string>, "Type T is not a class!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
}

#if defined(MIRROR_COLLECTION_STD_ARRAY) && MIRROR_COLLECTION_STD_ARRAY
template <typename T>
static void SetCollectionLambdasArray(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasArray(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_array<T>::value, "Type T is not of type stl::array!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t index, const void* elementFirst, const void* /*elementSecond*/) {
		T* arr = ((T*)collectionAddress);
		// #TODO Safety checks: assert(index < arr->size());
		// arr[index] = (*(typename T::value_type*)elementFirst);
		arr[index] = (*(typename T::value_type*)elementFirst);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0; // #TODO Support iterating backwards over a collection, and random access
		T* vector = ((T*)collectionAddress);
		if (aIndex < index) index = aIndex;
		if (index >= vector->size()) { index = 0; return nullptr; }
		return (char*)vector->data() + (sizeof(T::value_type) * index++);
	};
}
#endif // defined(MIRROR_COLLECTION_STD_ARRAY) && MIRROR_COLLECTION_STD_ARRAY

#if defined(MIRROR_COLLECTION_STD_DEQUE) && MIRROR_COLLECTION_STD_DEQUE
template <typename T>
static void SetCollectionLambdasDeque(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasDeque(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	// static_assert(false, "Function not yet implemented!");

	static_assert(is_stl_deque<T>::value, "Type T is not of type stl::deque!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->push_back(*(typename T::value_type*)elementFirst);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		// #TODO Implement
		return nullptr;
	};
}
#endif // defined(MIRROR_COLLECTION_STD_DEQUE) && MIRROR_COLLECTION_STD_DEQUE

#if defined(MIRROR_COLLECTION_STD_FORWARD_LIST) && MIRROR_COLLECTION_STD_FORWARD_LIST
template <typename T>
static void SetCollectionLambdasForwardList(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasForwardList(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_forward_list<T>::value, "Type T is not of type stl::forward_list!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		// #TODO Add to forward_list
	};
	GENERIC_ITERATE_LAMBDA
}
#endif // defined(MIRROR_COLLECTION_STD_FORWARD_LIST) && MIRROR_COLLECTION_STD_LIST

#if defined(MIRROR_COLLECTION_STD_LIST) && MIRROR_COLLECTION_STD_LIST
template <typename T>
static void SetCollectionLambdasList(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasList(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_list<T>::value, "Type T is not of type stl::list!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		T* list = (T*)collectionAddress;
		// #TODO Use iterator: auto iterator = list.begin();
	};
	GENERIC_ITERATE_LAMBDA
}
#endif // defined(MIRROR_COLLECTION_STD_LIST) && MIRROR_COLLECTION_STD_LIST

#if defined(MIRROR_COLLECTION_STD_MAP) && MIRROR_COLLECTION_STD_MAP
template <typename T>
static void SetCollectionLambdasMap(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template <typename T>
static void SetCollectionLambdasMap(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_map<T>::value, "Type T is not a map!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->insert(*(typename T::value_type*)elementFirst);
	};
	GENERIC_ITERATE_LAMBDA
}
#endif // defined(MIRROR_COLLECTION_STD_MAP) && MIRROR_COLLECTION_STD_MAP

#if defined(MIRROR_COLLECTION_STD_MULTI_MAP) && MIRROR_COLLECTION_STD_MULTI_MAP
template <typename T>
static void SetCollectionLambdasMultiMap(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasMultiMap(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_multi_map<T>::value, "Type T is not of type std::multimap!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->insert(*(typename T::value_type*)elementFirst);
	};
	GENERIC_ITERATE_LAMBDA
}
#endif // defined(MIRROR_COLLECTION_STD_MULTI_MAP) && MIRROR_COLLECTION_STD_MULTI_MAP

#if defined(MIRROR_COLLECTION_STD_MULTI_SET) && MIRROR_COLLECTION_STD_MULTI_SET
template <typename T>
static void SetCollectionLambdasMultiSet(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasMultiSet(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_multi_set<T>::value, "Type T is not of type std::multiset!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		// #TODO multiset
		// ((T*)collectionAddress)->insert(*(typename T::value_type*)elementFirst);
	};
	GENERIC_ITERATE_LAMBDA
}
#endif // defined(MIRROR_COLLECTION_STD_MULTI_SET) && MIRROR_COLLECTION_STD_MULTI_SET

#if defined(MIRROR_COLLECTION_STD_QUEUE) && MIRROR_COLLECTION_STD_QUEUE
template <typename T>
static void SetCollectionLambdasQueue(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasQueue(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	// static_assert(false, "Function not yet implemented!");

	static_assert(is_stl_queue<T>::value, "Type T is not of type std::queue!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->push(*(typename T::value_type*)elementFirst);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		// #TODO std::queue does not support random read. May be forced to copy the queue to serialize all elements
		return nullptr;
	};
}
#endif // defined(MIRROR_COLLECTION_STD_QUEUE) && MIRROR_COLLECTION_STD_QUEUE

#if defined(MIRROR_COLLECTION_STD_SET) && MIRROR_COLLECTION_STD_SET
template <typename T>
static void SetCollectionLambdasSet(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasSet(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_set<T>::value, "Type T is not of type std::set!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		// #TODO set
		// ((T*)collectionAddress)->insert(*(typename T::value_type*)elementFirst);
	};
	GENERIC_ITERATE_LAMBDA
}
#endif // defined(MIRROR_COLLECTION_STD_SET) && MIRROR_COLLECTION_STD_SET

#if defined(MIRROR_COLLECTION_STD_STACK) && MIRROR_COLLECTION_STD_STACK
template <typename T>
static void SetCollectionLambdasStack(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasStack(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_stack<T>::value, "Type T is not of type std::stack!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		// #TODO stack
		// ((T*)collectionAddress)->insert(*(typename T::value_type*)elementFirst);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		// #TODO stack
		// static size_t index = 0;
		// T* map = (T*)collectionAddress;
		// static T::iterator iterator = map->begin();
		// if (aIndex < index || map->end() == iterator)
		// {
		// 	index = aIndex;
		// 	iterator = map->begin();
		// }
		// if (index >= map->size()) { ++index; return nullptr; }
		// ++index;
		// auto result = (char*)&iterator->first;
		// ++iterator;
		// return result;
		return nullptr;
	};
}
#endif // defined(MIRROR_COLLECTION_STD_STACK) && MIRROR_COLLECTION_STD_STACK

#if defined(MIRROR_COLLECTION_STD_PAIR) && MIRROR_COLLECTION_STD_PAIR
template <typename T>
static void SetCollectionLambdasPair(Mirror::TypeInfo* constTypeInfo, std::false_type) { }

template <typename T>
static void SetCollectionLambdasPair(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_pair<T>::value, "Type T is not a pair!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::first_type>());
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::second_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::first_type>();
	mutableTypeInfo->collectionTypeInfoSecond = Mirror::InfoForType<T::second_type>();

	mutableTypeInfo->collectionAddFunc = [](void* pairObjAddress, size_t /*aIndex*/, const void* elementFirst, const void* elementSecond) {
		T* pair = (T*)pairObjAddress;
		// #TODO Below commented code can be reviewed for deprecation
		// #TODO May need to remove elementSecond and use aIndex to determine 1st or 2nd type
		// if (0 == aIndex) { memcpy((void*)&pair->first, elementFirst, sizeof(T::first_type)); } else { memcpy((void*)&pair->second, elementSecond, sizeof(T::second_type)); }
		memcpy((void*)&pair->first, elementFirst, sizeof(T::first_type));
		memcpy((void*)&pair->second, elementSecond, sizeof(T::second_type));
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* pairObjAddress, size_t aIndex) -> char* {
		T* pair = (T*)pairObjAddress;
		if (0 == aIndex) return (char*)&pair->first;
		else if (1 == aIndex) return (char*)&pair->second;
		else return nullptr;
	};
}
#endif // defined(MIRROR_COLLECTION_STD_PAIR) && MIRROR_COLLECTION_STD_PAIR

#if defined(MIRROR_COLLECTION_STD_PRIORITY_QUEUE) && MIRROR_COLLECTION_STD_PRIORITY_QUEUE
template <typename T>
static void SetCollectionLambdasPriorityQueue(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasPriorityQueue(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_priority_queue<T>::value, "Type T is not of type std::priority_queue!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::first_type>());
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::second_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();

	mutableTypeInfo->collectionAddFunc = [](void* pairObjAddress, size_t /*aIndex*/, const void* elementFirst, const void* elementSecond) {
		// #TODO priority
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* pairObjAddress, size_t aIndex) -> char* {
		// #TODO priority
		return nullptr;
	};
}
#endif // defined(MIRROR_COLLECTION_STD_PRIORITY_QUEUE) && MIRROR_COLLECTION_STD_PRIORITY_QUEUE

#if defined(MIRROR_COLLECTION_STD_TUPLE) && MIRROR_COLLECTION_STD_TUPLE
template <typename T>
static void SetCollectionLambdasTuple(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

// #TODO lambdas loop over args

template <typename T, typename... TupleType>
void SetCollectionLambdasTuple_Singular(T tuple, std::vector<uint64_t>& tupleVec, Mirror::TypeInfo* mutableTypeInfo)
{
	static int counter = 0; // #TODO Will index always match the type? Order dependency
	([&]()
	{
		char* tupleAddress = &tuple;
		char* tupleTypeAddress = &std::get<counter>(tuple);
		tupleVec.emplace_back(tupleTypeAddress - tupleAddress); // #TODO Review dynamic emplace back allocation(s)
		mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<TupleType>());
	}(), ...);
}

template <typename T, typename... TupleType>
static void SetCollectionLambdasTuple_Plural(Mirror::TypesList<TupleType...>, std::vector<uint64_t>& tupleVec, Mirror::TypeInfo* mutableTypeInfo)
{
	static T tuple; // #TODO Need to pass?
	SetCollectionLambdasTuple_Singular<TupleType...>(tuple, tupleVec, mutableTypeInfo);
}

template <typename T>
static void SetCollectionLambdasTuple(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(false, "Tuple not currently supported!");
	static_assert(is_stl_tuple<T>::value, "Type T is not a tuple!");

	// #TODO Look at examples from: https://en.cppreference.com/w/cpp/utility/tuple/tuple
	// template<class... Args>
	// void print(std::string_view message, const std::tuple<Args...>& t)
	// Expanding tuple arguments to send to SetCollectionLambdasTuple_Plural()

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionOffsetsVecFunc = []() -> const std::vector<size_t>* {
		static std::vector<size_t> tupleIndexOffsets;
		if (tupleIndexOffsets.empty())
		{
			Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(Mirror::InfoForType<T>());
			// SetCollectionLambdasTuple_Plural<T>(Mirror::TypesList { T... }, tupleIndexOffsets, mutableTypeInfo);
		}
		return &tupleIndexOffsets;
	};
	mutableTypeInfo->collectionAddFunc = [](void* tupleObjAddress, size_t aIndex, const void* elementFirst, const void* elementSecond) -> void {
		T* tuple = (T*)tupleObjAddress;
		const Mirror::TypeInfo* immutableTypeInfo = Mirror::InfoForType<T>(); // #NOTE Capturing outside mutableTypeInfo changes function pointer signature
		char* tupleIndexAddress = (char*)&std::get<0>(*tuple);
		tupleIndexAddress += immutableTypeInfo->size; // #TODO Get size, maybe by looping over values
		memcpy((void*)tupleIndexAddress, elementFirst, immutableTypeInfo->collectionTypeInfos[aIndex]->size);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* tupleObjAddress, size_t aIndex) -> char* {
		// T* tuple = (T*)tupleObjAddress;
		const Mirror::TypeInfo* immutableTypeInfo = Mirror::InfoForType<T>(); // #NOTE Capturing outside mutableTypeInfo changes function pointer signature
		const std::vector<size_t>* tupleIndexOffsets = immutableTypeInfo->collectionOffsetsVecFunc();
		if (aIndex < immutableTypeInfo->collectionTypeInfos.size()) { return (char*)(tupleObjAddress) + tupleIndexOffsets->at(aIndex); }
		return nullptr;
	};
	mutableTypeInfo->typeConstructorFunc = [](void* preallocatedMemoryAddress) { new(preallocatedMemoryAddress) T; };
}
#endif // defined(MIRROR_COLLECTION_STD_TUPLE) && MIRROR_COLLECTION_STD_TUPLE

#if defined(MIRROR_COLLECTION_STD_VECTOR) && MIRROR_COLLECTION_STD_VECTOR
template <typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::false_type) {}

template <typename T>
static void SetCollectionLambdasVector(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_vector_impl::is_stl_vector<T>::type(), "Type T is not a vector!");

	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
	mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<T::value_type>());
	mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<T::value_type>();
	mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t /*index*/, const void* elementFirst, const void* /*elementSecond*/) {
		((T*)collectionAddress)->emplace_back(*(typename T::value_type*)elementFirst);
	};
	mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
		static size_t index = 0; // #TODO Support iterating backwards over a collection, and random access
		T* vector = ((T*)collectionAddress);
		if (aIndex < index) index = aIndex;
		if (index >= vector->size()) { index = 0; return nullptr; }
		return (char*)vector->data() + (sizeof(T::value_type) * index++);
	};
	// #TODO Pros/cons of unique versus generic macro implementation
	// GENERIC_ITERATE_LAMBDA
}
#endif // defined(MIRROR_COLLECTION_STD_VECTOR) && MIRROR_COLLECTION_STD_VECTOR

template <typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo, std::false_type) {
	if (std::is_array_v<T>)
	{
		Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);
		typedef typename std::remove_all_extents<T>::type ArrayElementType;
		mutableTypeInfo->collectionTypeInfos.emplace_back(Mirror::InfoForType<ArrayElementType>());
		mutableTypeInfo->collectionTypeInfoFirst = Mirror::InfoForType<ArrayElementType>();
		mutableTypeInfo->collectionAddFunc = [](void* collectionAddress, size_t index, const void* elementFirst, const void* /*elementSecond*/) {
			memcpy((char*)collectionAddress + (sizeof(ArrayElementType) * index), elementFirst, sizeof(ArrayElementType));
		};
		mutableTypeInfo->collectionIterateCurrentFunc = [](const void* collectionAddress, size_t aIndex) -> char* {
			static size_t index = 0;
			if (aIndex < index) index = aIndex;
			if (index >= sizeof(T) / sizeof(ArrayElementType)) { index = 0; return nullptr; }
			return (char*)collectionAddress + (sizeof(ArrayElementType) * index++);
		};
	}
}

template <typename T>
static void SetCollectionLambdas(Mirror::TypeInfo* constTypeInfo, std::true_type) {
	static_assert(is_stl_container<T>::value, "Type T is not a collection!");
	Mirror::TypeInfo* mutableTypeInfo = const_cast<Mirror::TypeInfo*>(constTypeInfo);

#ifdef MIRROR_COLLECTION_STD_ARRAY
	SetCollectionLambdasArray<T>(mutableTypeInfo, is_stl_array_impl::is_stl_array<T>::type());
#endif // MIRROR_COLLECTION_STD_ARRAY

#ifdef MIRROR_COLLECTION_STD_DEQUE
	SetCollectionLambdasDeque<T>(mutableTypeInfo, is_stl_deque_impl::is_stl_deque<T>::type());
#endif // MIRROR_COLLECTION_STD_DEQUE

#ifdef MIRROR_COLLECTION_STD_FORWARD_LIST
	SetCollectionLambdasForwardList<T>(mutableTypeInfo, is_stl_forward_list_impl::is_stl_forward_list<T>::type());
#endif // MIRROR_COLLECTION_STD_FORWARD_LIST

#ifdef MIRROR_COLLECTION_STD_LIST
	SetCollectionLambdasList<T>(mutableTypeInfo, is_stl_list_impl::is_stl_list<T>::type());
#endif // MIRROR_COLLECTION_STD_LIST

#ifdef MIRROR_COLLECTION_STD_MAP
	SetCollectionLambdasMap<T>(mutableTypeInfo, is_stl_map_impl::is_stl_map<T>::type());
#endif // MIRROR_COLLECTION_STD_MAP

#ifdef MIRROR_COLLECTION_STD_MULTI_MAP
	SetCollectionLambdasMultiMap<T>(mutableTypeInfo, is_stl_multi_map_impl::is_stl_multi_map<T>::type());
#endif // MIRROR_COLLECTION_STD_MULTI_MAP

#ifdef MIRROR_COLLECTION_STD_MULTI_SET
	SetCollectionLambdasMultiSet<T>(mutableTypeInfo, is_stl_multi_set_impl::is_stl_multi_set<T>::type());
#endif // MIRROR_COLLECTION_STD_MULTI_SET

#ifdef MIRROR_COLLECTION_STD_QUEUE
	SetCollectionLambdasQueue<T>(mutableTypeInfo, is_stl_queue_impl::is_stl_queue<T>::type());
#endif // MIRROR_COLLECTION_STD_QUEUE

#ifdef MIRROR_COLLECTION_STD_SET
	SetCollectionLambdasSet<T>(mutableTypeInfo, is_stl_set_impl::is_stl_set<T>::type());
#endif // MIRROR_COLLECTION_STD_SET

#ifdef MIRROR_COLLECTION_STD_STACK
	SetCollectionLambdasStack<T>(mutableTypeInfo, is_stl_stack_impl::is_stl_stack<T>::type());
#endif // MIRROR_COLLECTION_STD_STACK

#ifdef MIRROR_COLLECTION_STD_PAIR
	SetCollectionLambdasPair<T>(mutableTypeInfo, is_stl_pair_impl::is_stl_pair<T>::type());
#endif // MIRROR_COLLECTION_STD_PAIR

#ifdef MIRROR_COLLECTION_STD_PRIORITY_QUEUE
	SetCollectionLambdasPriorityQueue<T>(mutableTypeInfo, is_stl_priority_queue_impl::is_stl_priority_queue<T>::type());
#endif // MIRROR_COLLECTION_STD_PRIORITY_QUEUE

#ifdef MIRROR_COLLECTION_STD_TUPLE
	SetCollectionLambdasTuple<T>(mutableTypeInfo, is_stl_tuple_impl::is_stl_tuple<T>::type());
#endif MIRROR_COLLECTION_STD_TUPLE

#ifdef MIRROR_COLLECTION_STD_VECTOR
	SetCollectionLambdasVector<T>(mutableTypeInfo, is_stl_vector_impl::is_stl_vector<T>::type());
#endif // MIRROR_COLLECTION_STD_VECTOR
}
