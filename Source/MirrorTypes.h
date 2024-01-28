#pragma once

// #NOTE Maintain order in-dependence by avoiding writing enum values to data, or relying on their static values (0,1...etc) in code
enum class MirrorTypes {

    // User defined types
    // Add type names here to support Mirror reflection (case sensitive)
    ExampleStruct, // <- Remove this test value as you wish
    ExampleClass, // <- Remove this test value as you wish
    ExampleNestedCutomTypes,

    // Primitive types. #NOTE A prefix is used to avoid naming collisions with standard type keywords
    m_PRIMITIVE_TYPES,
    m_int8_t,
    m_int16_t,
    m_int32_t,
    m_int64_t,

    m_uint8_t,
    m_uint16_t,
    m_uint32_t,
    m_uint64_t,

    m_int,
    m_bool,
    m_char,
    m_float,
    m_double,
    m_charPtr,
    m_constCharPtr,
    m_string
};
