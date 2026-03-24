Current issue:
It's not possible to use MIR_CLASS() and inside register derived and base types at the same time.
The reason is that both types must finish defining themselves before they can each register.

Inside of a child MIR_CLASS(), we try to set the derivedTypes vector with TypeInfo* to undefined child types.
That won't work if the child types also try to set their superTypeInfo to be the yet to finish defining base type.
If the child types don't immediately try to set superTypeInfo then the the types should resolve.

The child types will still need their super type set explictely, so that is where the solutions start:
- Ideal: Within 1 MIR_CLASS() for the base type and 1 MIR_CLASS() for each child type, resolve all TypeInfo* references
- Middle ground: Define parent MIR_CLASS() OR children MIR_CLASS() calls and then define the remaining type(s) while also setting references inside of only MIR_CLASS() as dependent types have been defined
- Worst: Define parent and child (no order dependency), then make sepearate calls to set references in parent and children

### Option
```
MIR_CLASS(DERIVED1)
MIR_CLASS(DERIVED2)

MIR_CLASS(BASE)
MIR_DERIVED_TYPE(DERIVED1) // Sets self reference to child, and child's reference to parent
MIR_DERIVED_TYPE(DERIVED2)
```
Easy to forget to update the base class when adding new child classes.
Maybe the parent could be defined 1st which may (not sure how) also avoid other order dependencies.
So perhaps like:
```
MIR_CLASS(BASE) // No child registration/awareness

MIR_CLASS(DERIVED1)
MIR_CLASS_BASE(BASE) // Assigns this type parent pointer to BASE, and ads self type as derived in parent derived type list (review API)

MIR_CLASS(DERIVED2)
MIR_CLASS_BASE(BASE)

etc...
```
Either way, there is an implicit order dependency which is lame, but maybe still better than
needing additional code outside of all MIR_CLASS scope

### Option: GetDerivedTypes() function
Considering a GetDerivedTypes() function for delayed reference setting.
Not ideal as there would need to be hidden access to the vector and an if check each access.
Not an otion I really want to consider and finish implementing as of now.

#define MIR_CLASS_BASE(BASE_TYPE, DERIVED_TYPE)																								\
const Mirror::TypeInfo* BASE_TYPE##Info = Mirror::InfoForType<BASE_TYPE>();																	\
const Mirror::TypeInfo* DERIVED_TYPE##Info = Mirror::InfoForType<DERIVED_TYPE>();															\
BASE_TYPE##Info.derivedTypes.push_back(DERIVED_TYPE##Info);

// struct TypesList {}; // From MIR_Structs.h

class MyChildClass { };

#define MIR_DERIVED_TYPES(x) \
using DerivedTypes = Mirror::TypesList<x>; // #TODO DerivedTypes needs to be accessible and defined or no compilation

// Usage
MIR_DERIVED_TYPES(MyChildClass)

#define MIR_					\
template <typename... DerivedType>																											\
void SetDerivedTypePointer(std::vector<const Mirror::TypeInfo*>& derivedTypes) {															\
	[]() {																																	\
		derivedTypes.push_back(DERIVED_TYPE##Info);																							\
	}																																		\
}																																			\
template<typename... DerivedTypes>																											\
void SetDerivedTypePointers(TemplateArgumentList<DerivedTypes...>, std::vector<const Mirror::TypeInfo*>& derivedTypes) {					\
	SetDerivedTypePointer<DerivedTypes...>(derivedTypes);																					\
}
if (BASE_TYPE##Info.derivedTypes.empty()) {	\
		SetDerivedTypePointers(DerivedTypes{}, BASE_TYPE##Info.derivedTypes);	\
}
```