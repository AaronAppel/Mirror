#include <map>
#include <string>
#include <vector>

#include "MIR_Mirror.h"

#include "main.h"

#include "MirrorTypes.h"

// Declare custom types to generate type info functions
MIRROR_TYPE(float[10])
MIRROR_TYPE(std::vector<char>)
MIRROR_TYPE(std::pair<const int, bool>)
MIRROR_TYPE(std::map<int, bool>)

MIRROR_CLASS(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intZ)
MIRROR_CLASS_END

MIRROR_CLASS(ExampleClass)
MIRROR_CLASS_SUBCLASS(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intA);
MIRROR_CLASS_MEMBER(boolB);
MIRROR_CLASS_MEMBER(charC);
MIRROR_CLASS_MEMBER(floatD);
MIRROR_CLASS_MEMBER(doubleE);
MIRROR_CLASS_MEMBER(constCharPtrF);
MIRROR_CLASS_MEMBER(stdStringG);
MIRROR_CLASS_MEMBER(exampleMapH);
MIRROR_CLASS_END

MIRROR_CLASS(ExampleNestedCustomTypes)
MIRROR_CLASS_MEMBER(exClass)
MIRROR_CLASS_END

#ifdef MIRROR_DEV_EXPERIMENTAL

// #TODO Find a way to simplify mirroring derived types and accept templated arguments in a type-agnostic way
template<typename... T>
struct TemplateArgumentList {};
// Or
using MirrorList = Mirror::MirrorTemplateArgumentList<
    bool,
    int
>;

// #TODO Finalize design/API for pre-defined template listed types
template<typename... SubClass>
static void TemplateArgs()
{
    Mirror::MirrorTemplateArgumentList<SubClass...> list;
}

template<typename... SubClass>
static void TemplateArgs(TemplateArgumentList<SubClass...>)
{
    Mirror::MirrorTemplateArgumentList<SubClass...> list;
}

template <typename SuperClass, typename... SubClass>
static void MirrorSubClass(void* localStaticTypeInfo, uint16_t enumStartOffset)
{
    uint16_t enumValue = enumStartOffset;
    ([&]()
        {
            ++enumValue;
        }(), ...);
}
#endif