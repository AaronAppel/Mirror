#pragma once

// #NOTE Maintain order in-dependence by avoiding writing enum values to data, or relying on their static values (0,1...etc) in code
enum class MirrorTypes
{
    m_Invalid = 0,

#ifdef MIRROR_EXAMPLES // Remove example values as you wish
    // Library example user defined types (case sensitive)
    ExampleStruct,
    ExampleClass,
    ExampleDerivedClass,
    ExampleNestedCutomTypes,
#endif

    // User defined types (case sensitive)
    // YourTypeHere,

    m_PRIMITIVE_TYPES,
#include "MirrorEnumPrimitives.txt"
    m_PRIMITIVES_END,
};
