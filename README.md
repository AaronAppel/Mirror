⚠️ Refactor in progress. Documentation out of date ⚠️

# Mirror
A super simple C++ reflection library I made to enable serializing fields in my game engine [QwerkE](https://github.com/AaronAppel/QwerkE)

The implementation was initially inspired by work from the GitHub user danbar0, and their repo : https://github.com/danbar0/StaticJsonReflection

The code isn't extensively tested, so just be aware.

Hopefully someone finds Mirror helpful.
Thanks for stopping by :)

#TODO 
- Decide on opening '{' placement in all files for consistency.
- \#endif comments
# Usage
1. Create your custom class/struct :

```
#include "Mirror.h"
class ExampleClass
{
public:
	int intX = 9;

private:
	MIRROR_PRIVATE_MEMBERS
	int intY = 4;
};
```

Here, MIRROR_PRIVATE_MEMBERS exposes the private value intY to the Mirror struct for visibility. Only required for private or protected class fields.

2. Mirror the type info (tell Mirror what your type looks like) :

```
MIRROR_CLASS_START(ExampleClass)
// MIRROR_CLASS_MEMBER(intX) // Not currently mirrored by default
MIRROR_CLASS_MEMBER(intY) // Explicitly mirrored
MIRROR_CLASS_END(ExampleClass)

MIRROR_TYPE(float[10]) // Example for non-class
```

We can omit/ignore class fields we don't need exposed (not serialized for example).

3. Mirror the type ID (tell Mirror what value to use in switch statements, etc) :
```
MIRROR_TYPE_ID(1, ExampleClass)
```
Notes for mirroring type IDs:
- Organizing type by categories can help manage growth over time, but ordering does not matter
- Be sure to manage serialized type IDs so their values stay synced between code and data
- At this time Mirror cannot check that an ID value is already in use, so try to avoid duplication or copy/paste errors
- MIRROR_TYPE_ID_SIZE_MAX is the largest value an ID can be

4. Get type info and type ID at runtime for the type :
```
ExampleClass exampleClass;

const Mirror::TypeInfo* exampleClassTypeInfo = Mirror::InfoForType<ExampleClass>();
// or by object reference
const Mirror::TypeInfo* exampleClassTypeInfo = Mirror::InfoForType(exampleClass);

MIRROR_TYPE_ID_TYPE exampleClassTypeID = Mirror::TypeId<ExampleClass>();
// or by object reference
MIRROR_TYPE_ID_TYPE exampleClassTypeID = Mirror::TypeId<>(exampleClass);
```
MIRROR_TYPE_ID_TYPE is currently a uint8_t, but can always change.
You can change the type at the top of MIR_Structs.h, as you wish.

5. Use type info to serialize type info fields :
```
const Mirror::TypeInfo* exampleClassTypeInfo = Mirror::InfoForType<ExampleClass>();

std::cout << "ExampleClass Mirrored Fields:\n";
for (size_t i = 0; i < exampleClassTypeInfo->fields.size(); i++)
{
	const Mirror::Field& field = exampleClassTypeInfo->fields[i];
	switch (field.typeInfo->enumType)
	{
	case MirrorTypes::m_int32_t:
		std::cout << "m_int32_t" << " : " << field.name.c_str() << "\n";
		break;
		// ...
	}
}
```
In [QwerkE](https://github.com/AaronAppel/QwerkE), I also use this reflection and serialization technique in object inspectors for viewing and editing object values using UI like [dear imgui](https://github.com/ocornut/imgui)

# Usage (Updated)
MIRROR_TYPE_ID(0, MyClass)
MIRROR_TYPE_ID(1, MyStruct)
... etc

\#NOTE Serialized types
If you are going to serialize type IDs by saving them in a file or persistent data, it is recommended that you choose static IDs that will not change over time. A simple way to do this is to use the MIRROR_TYPE_ID_IMPL(ID, TYPE) macro and supply a fixed ID value, and mirror this value before all non-serialized type IDs. This is also recommended when using MIRROR_NONCONFORMING and MIRROR_GENERATE_TYPE_IDS options.

# Building the examples project
Run Generate-Win-Vs2022.bat to generate a Visual Studio 2022 solution using Premake5.
See # FAQ and Common Errors section below for more info
# Macros
#### User Macros
#TODO Add information for each macro
MIRROR_DEBUG
MIRROR_TYPE_SIZE_UNUSED
MIRROR_FIELD_FLAGS_UNUSED

#### Additional Macros:
MIRROR_PRIVATE_MEMBERS

MIRROR_FIELD_FLAG_SIZE
MIRROR_FIELD_FLAG_SIZE_MAX
MIRROR_FIELD_ID_SIZE
MIRROR_FIELD_ID_SIZE_MAX
MIRROR_TYPE_SIZE
MIRROR_TYPE_SIZE_MAX
MIRROR_TYPE_CATEGORY_SIZE
MIRROR_TYPE_CATEGORY_SIZE_MAX
MIRROR_MEMBER_FIELDS_COUNT_DEFAULT

MIRROR_USER_TYPE_ID_MAX (MIRROR_FIELD_ID_SIZE_MAX - 58)
# Limitations
### Compiler Considerations
Mirror is currently developed using MSVC compiler. No efforts have been made to compile on GCC, Clang, or other compilers, at this time.
Builds with strict conformance (/permissive-) fail due to template compiler Error C2672 in multiple MIR_Mirror.h functions.
### Unsupported Types:
• Object references
• Multi-dimensional pointers
• Multi-dimensional containers
• STL containers (excluding vector, map, pair, and other types included in MIR_TypeDeduction.h)

• Also, multiple inheritance is not currently supported

# FAQ and Common Errors
#TODO List expected/common errors and their solutions
- Conformance command line argument /permissive compile errors
- Missing/forgot to mirror type or mirror type id linker errors
