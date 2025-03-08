# Mirror
A super simple C++ reflection library I made to enable serializing fields in my game engine [QwerkE](https://github.com/AaronAppel/QwerkE)

The implementation was inspired by work from the GitHub user danbar0, and their repo : https://github.com/danbar0/StaticJsonReflection

The code isn't extensively tested, so just be aware.

Hopefully someone finds Mirror helpful.
Thanks for stopping by :)

# Building
Currently, the build system I use is premake5, which is included inside of the Scripts/ folder.
To build a Visual Studio 2022 solution on Windows, run the Scripts/Setup-Windows-Vs2022.bat file.
See [[#Known Working Compiler Versions]] for tested compiler versions.
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

Here, the MIRROR_PRIVATE_MEMBERS exposes the private value intY to the Mirror struct for visibility. Only required for private or protected members.

2. Mirror the type info (tell Mirror what your type looks like) :

```
MIRROR_CLASS_START(ExampleClass)
// Not serialized MIRROR_CLASS_MEMBER(intX)
MIRROR_CLASS_MEMBER(intY)
MIRROR_CLASS_END(ExampleClass)
```

We can omit/ignore values we don't need exposed (not serialized for example).

3. Add an entry into the MirrorTypes.h enum :
```
// MirrorTypes.h
enum class MirrorTypes
{
//	...
	ExampleClass,
//	...
}
```
Type names can collide, so be sure to watch case sensitivity and avoid primitives and keywords like int.

4. Get type info at runtime for the type :
```
const Mirror::TypeInfo* exampleClassTypeInfo = Mirror::InfoForType<ExampleClass>();
// or by object reference

ExampleClass exampleClass;
const Mirror::TypeInfo* exampleClassTypeInfo = Mirror::InfoForType(exampleClass);
```

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
In [QwerkE](https://github.com/AaronAppel/QwerkE), I also use this reflection and serialization technique in object inspectors for viewing and editing values using UI like [dear imgui](https://github.com/ocornut/imgui)

# Macros
#### User Macros
#TODO Add information for each macro
MIRROR_TESTING
MIRROR_NONCONFORMING
MIRROR_GENERATE_TYPE_IDS
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

# Limitations
### Compiler Considerations
Mirror is currently developed using MSVC compiler. No efforts have been made to compile on GCC, Clang, or other compilers, at this time.
MSVC compiler versions have been known to change critical features that prevent Mirror from compiling.
Use with caution, and see [[#Known Working Compiler Versions]] as outlined below.
### Unsupported Types:
• References
• Multi-dimensional pointers
• Multi-dimensional containers
• STL containers (excluding vector, map, pair)

# Known Working Compiler Versions

| Compiler | Version | Strict Conformance |
| -------- | ------- | ------------------ |
| MSVC     | 1938    | No                 |
| MSVC     | 1942    | Yes                |
| MSVC     | 1943    | Yes                |

#TODO Add more info about compiler conformance considerations.
Compiler standard conformance mode, or "/permissive(-)" command line argument.

See "ConstexprCounter.h" for non-conforming code section.

# FAQ and Common Errors
#TODO List expected/common errors and their solutions
- Conformance command line argument /permissive compile errors
- Missing/forgot to mirror type or mirror type id linker errors