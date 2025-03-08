# Dev Notes
This file is to keep track of thoughts I have along Mirror's development.
Feel free to ignore or peruse.

# Optimizations
### Unions for contextual struct pointers

Depending on the type that a TypeInfo struct is holding, there will be unused TypeInfo* fields, such as the following:

std::vector\<Field\> fields = { }; // #NOTE Only required for classes
std::vector\<const TypeInfo*\> derivedTypes; // #NOTE Only required for base classes (with declared derived types)
		
const TypeInfo* collectionTypeInfoFirst = nullptr; // #NOTE Only used for collections
const TypeInfo* collectionTypeInfoSecond = nullptr; // #NOTE Only used for collections
const TypeInfo* superTypeInfo; // #NOTE Only used for derived types
const TypeInfo* pointerDereferencedTypeInfo; // #NOTE Only used for pointers

FuncPtr_voidPtr_constVoidPtr_bool collectionAddressOfPairObjectFunc = nullptr; // #NOTE Only used for collections
FuncPtr_void_voidPtr collectionClearFunction = nullptr; // #NOTE Only used for collections
FuncPtr_charPtr_constVoidPtr_sizet collectionIterateCurrentFunc = nullptr; // #NOTE Only used for collections

FuncPtr_void_voidPtr typeConstructorFunc = nullptr; // #NOTE Only used for classes and collections
FuncPtr_bool_constVoidPtr typeDynamicCastFunc = nullptr; // #NOTE Only used for classes in inheritance hierarchy
		
These fields are contextual, or only used for certain types.

I had looked at using C++ unions to share memory and reduce unused memory. Ultimately I abandoned the idea as I didn't like the hit to readability, and I doubt anyone else will use this library, or that my side projects will be impacted by the small misuse of ram.

This note exists instead, for potential future reference.

## Member field alignment and packing
I am no expert in memory alignment and packing, but I do know that re-arranging fields could see benefits like reduced class instance size, specifically for the TypeInfo struct. For now I won't dive into it but will note the thought here instead.

# Key Considerations
### TypeId value consistency and compile time usage
In order for all compile units to have the same id for types, type ids must be declared in the same way, or in the same scope. The current implementation as of March 5th, 2025, is using templated constexpr functions that evaluate to a std::size_t. Calling the function for a given type will guarantee the value is the same across all calls and can even be used for compile time evaluations like switch statement cases.

### Special cases to avoid sizeof(void) compilation error
#TODO Consider expanding information.
See MIRROR_TYPE_NON_VOID for more info.

### Lambdas methods must be split
Vector and Map lambdas SetCollectionLambdasVector, and SetCollectionLambdasMap differ slightly by push_back/insert calls and iteration logic. A combined function won't compile because the compiler will flag missing methods as errors, even when using templated if logic to avoid runtime execution.
