
#include "map"
#include "string"
#include "unordered_map"
#include "vector"

#include "Mirror.h"

#include "TypeIdsForMirror.h"
#include "Serialization/MirrorTesting.h"

#include "ConstexprCounter.h"

// Define implementation of Mirror::InfoForType<TYPE>() for each user created class, or type(s) unsupported by Mirror as default

// #TODO Clean up void specific (avoid sizeof()) implementation
template<> static const Mirror::TypeInfo* Mirror::InfoForType<void>() {
    static Mirror::TypeInfo localStaticTypeInfo; if (!localStaticTypeInfo.stringName.empty()) {
        return &localStaticTypeInfo;
    }
    localStaticTypeInfo.category = GetCategory<void>();
    localStaticTypeInfo.id = Mirror::TypeId<void>(); localStaticTypeInfo.stringName = typeid(void).name();
    SetConstructionLambda<void>(&localStaticTypeInfo, std::is_same<void, std::string>::type());
    return &localStaticTypeInfo;
}
MIRROR_INFO_FOR_TYPE(void*)

// Mutable
MIRROR_INFO_FOR_TYPE(signed char)
MIRROR_INFO_FOR_TYPE(signed short)
MIRROR_INFO_FOR_TYPE(signed int)
MIRROR_INFO_FOR_TYPE(signed long long)
MIRROR_INFO_FOR_TYPE(unsigned char)
MIRROR_INFO_FOR_TYPE(unsigned short)
MIRROR_INFO_FOR_TYPE(unsigned int)
MIRROR_INFO_FOR_TYPE(unsigned long long)
MIRROR_INFO_FOR_TYPE(float)
MIRROR_INFO_FOR_TYPE(double)
MIRROR_INFO_FOR_TYPE(long double)
MIRROR_INFO_FOR_TYPE(char)
MIRROR_INFO_FOR_TYPE(bool)

// Const
MIRROR_INFO_FOR_TYPE(const signed char)
MIRROR_INFO_FOR_TYPE(const signed short)
MIRROR_INFO_FOR_TYPE(const signed int)
MIRROR_INFO_FOR_TYPE(const signed long long)
MIRROR_INFO_FOR_TYPE(const unsigned char)
MIRROR_INFO_FOR_TYPE(const unsigned short)
MIRROR_INFO_FOR_TYPE(const unsigned int)
MIRROR_INFO_FOR_TYPE(const unsigned long long)
MIRROR_INFO_FOR_TYPE(const float)
MIRROR_INFO_FOR_TYPE(const double)
MIRROR_INFO_FOR_TYPE(const long double)
MIRROR_INFO_FOR_TYPE(const char)
MIRROR_INFO_FOR_TYPE(const bool)

// Mutable pointers
MIRROR_INFO_FOR_TYPE(signed char*)
MIRROR_INFO_FOR_TYPE(signed short*)
MIRROR_INFO_FOR_TYPE(signed int*)
MIRROR_INFO_FOR_TYPE(signed long long*)
MIRROR_INFO_FOR_TYPE(unsigned char*)
MIRROR_INFO_FOR_TYPE(unsigned short*)
MIRROR_INFO_FOR_TYPE(unsigned int*)
MIRROR_INFO_FOR_TYPE(unsigned long long*)
MIRROR_INFO_FOR_TYPE(float*)
MIRROR_INFO_FOR_TYPE(double*)
MIRROR_INFO_FOR_TYPE(long double*)
MIRROR_INFO_FOR_TYPE(char*)
MIRROR_INFO_FOR_TYPE(bool*)

// Const pointers
MIRROR_INFO_FOR_TYPE(const signed char*)
MIRROR_INFO_FOR_TYPE(const signed short*)
MIRROR_INFO_FOR_TYPE(const signed int*)
MIRROR_INFO_FOR_TYPE(const signed long long*)
MIRROR_INFO_FOR_TYPE(const unsigned char*)
MIRROR_INFO_FOR_TYPE(const unsigned short*)
MIRROR_INFO_FOR_TYPE(const unsigned int*)
MIRROR_INFO_FOR_TYPE(const unsigned long long*)
MIRROR_INFO_FOR_TYPE(const float*)
MIRROR_INFO_FOR_TYPE(const double*)
MIRROR_INFO_FOR_TYPE(const long double*)
MIRROR_INFO_FOR_TYPE(const char*)
MIRROR_INFO_FOR_TYPE(const bool*)

// String
MIRROR_INFO_FOR_TYPE(std::string)
MIRROR_INFO_FOR_TYPE(std::string*)
MIRROR_INFO_FOR_TYPE(const std::string)
MIRROR_INFO_FOR_TYPE(const std::string*)

// TypeId Collections
MIRROR_INFO_FOR_TYPE(std::pair<const int, bool>)
MIRROR_INFO_FOR_TYPE(std::map<int, bool>)
MIRROR_INFO_FOR_TYPE(float[10])
MIRROR_INFO_FOR_TYPE(std::vector<char>)
MIRROR_INFO_FOR_TYPE(std::pair<std::string, int32_t>)
MIRROR_INFO_FOR_TYPE(std::unordered_map<std::string, int32_t>)
MIRROR_INFO_FOR_TYPE(std::pair<const std::string, int32_t>)

// Classes
template <typename Super, typename... SubClass>
void MirrorSubClassUserType(Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset)
{
    uint16_t enumValue = enumStartOffset;
    ([&]()
    {
        const Mirror::TypeInfo* subclassTypeInfo = Mirror::InfoForType<SubClass>();
        localStaticTypeInfo.derivedTypes.push_back(subclassTypeInfo);
        const_cast<Mirror::TypeInfo*>(subclassTypeInfo)->superTypeInfo = &localStaticTypeInfo;
        const_cast<Mirror::TypeInfo*>(subclassTypeInfo)->typeDynamicCastFunc =
            [](const void* pointerToInstance) -> bool {
            SubClass* subClass = (SubClass*)pointerToInstance;
            return dynamic_cast<SubClass*>(*(Super**)pointerToInstance) != nullptr;
            };
        ++enumValue;
    }(), ...);
}

template<typename Super, typename... T>
static void MirrorSubClassUserTypes(Mirror::MirrorTemplateArgumentList<T...>, Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset = 0)
{
    MirrorSubClassUserType<Super, T...>(localStaticTypeInfo, enumStartOffset);
}

MIRROR_CLASS_START(Derived1)
MIRROR_CLASS_MEMBER(derivedZ)
MIRROR_CLASS_END(Derived1)

MIRROR_CLASS_START(Derived2)
MIRROR_CLASS_MEMBER(derivedY)
MIRROR_CLASS_END(Derived2)

MIRROR_CLASS_START(Base)
MIRROR_CLASS_MEMBER(baseX)
MirrorSubClassUserTypes<Base>(Mirror::MirrorTemplateArgumentList<Derived1, Derived2>{}, localStaticTypeInfo, 0);
MIRROR_CLASS_END(Base)

MIRROR_CLASS_START(TestStruct)
MIRROR_CLASS_MEMBER(m_BasePtrDerived)
MIRROR_CLASS_MEMBER(m_Derived1Ptr)
MIRROR_CLASS_MEMBER(m_Derived2Ptr)
MIRROR_CLASS_MEMBER(m_Base)
MIRROR_CLASS_MEMBER(m_Derived1)
MIRROR_CLASS_MEMBER(m_Derived2)
MIRROR_CLASS_MEMBER(m_Bool)
MIRROR_CLASS_MEMBER(m_U8)
MIRROR_CLASS_MEMBER(m_U16)
MIRROR_CLASS_MEMBER(m_U32)
MIRROR_CLASS_MEMBER(m_U64)
MIRROR_CLASS_MEMBER(m_S8)
MIRROR_CLASS_MEMBER(m_S16)
MIRROR_CLASS_MEMBER(m_S32)
MIRROR_CLASS_MEMBER(m_S64)
MIRROR_CLASS_MEMBER(m_Float)
MIRROR_CLASS_MEMBER(m_Double)
MIRROR_CLASS_MEMBER(m_String)
MIRROR_CLASS_MEMBER(m_ConstCharPtr)
MIRROR_CLASS_MEMBER(m_FloatArray10)
MIRROR_CLASS_MEMBER(m_CharVector)
MIRROR_CLASS_MEMBER(m_UmapStringInt32)
MIRROR_CLASS_MEMBER(m_Int32Ptr)
MIRROR_CLASS_END(TestStruct)
