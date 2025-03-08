
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "MIR_Mirror.h"

#include "main_testing.h"

#include "MirrorTypeIds_Testing.h"
#include "MirrorTesting.h"

// Define implementation of Mirror::InfoForType<TYPE>() for each user created class, or type(s) unsupported by Mirror as default

// Type collections
MIRROR_TYPE(std::pair<const int, bool>)
MIRROR_TYPE(std::map<int, bool>)
MIRROR_TYPE(float[10])
MIRROR_TYPE(std::vector<char>)
MIRROR_TYPE(std::pair<std::string, int32_t>)
MIRROR_TYPE(std::unordered_map<std::string, int32_t>)
// #TODO Problems in SetConstructionLambda() MIRROR_TYPE(std::unordered_map<const std::string, int32_t>)
MIRROR_TYPE(std::pair<const std::string, int32_t>)

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
        const_cast<Mirror::TypeInfo*>(subclassTypeInfo)->typeDynamicCastFunc = [](const void* pointerToInstance) -> bool {
            return dynamic_cast<SubClass*>(*(Super**)pointerToInstance) != nullptr; // #TODO Fix warning C4541 "'dynamic_cast' used on polymorphic type 'Base' with /GR-; unpredictable behavior may result"
        };
        ++enumValue;
    }(), ...);
}

template<typename Super, typename... T>
static void MirrorSubClassUserTypes(Mirror::MirrorTemplateArgumentList<T...>, Mirror::TypeInfo& localStaticTypeInfo, uint16_t enumStartOffset = 0)
{
    MirrorSubClassUserType<Super, T...>(localStaticTypeInfo, enumStartOffset);
}

MIRROR_CLASS(ExampleStruct)
MIRROR_CLASS_MEMBER(intA);
MIRROR_CLASS_MEMBER(boolB);
MIRROR_CLASS_MEMBER(charC);
MIRROR_CLASS_MEMBER(floatD);
MIRROR_CLASS_MEMBER(doubleE);
MIRROR_CLASS_MEMBER(constCharPtrF);
MIRROR_CLASS_MEMBER(stdStringG);
MIRROR_CLASS_MEMBER(exampleMapH);
MIRROR_CLASS_END

MIRROR_CLASS(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intZ)
MIRROR_CLASS_END

MIRROR_CLASS(ExampleClass)
MIRROR_CLASS_SUBCLASS(ExampleDerivedClass)
MIRROR_CLASS_MEMBER(intX);
MIRROR_CLASS_MEMBER(intY);
MIRROR_CLASS_END

MIRROR_CLASS(ExampleNestedCustomTypes)
MIRROR_CLASS_MEMBER(exClass)
MIRROR_CLASS_END

MIRROR_CLASS(Derived1)
MIRROR_CLASS_MEMBER(derivedZ)
MIRROR_CLASS_END
MIRROR_TYPE(Derived1*)

MIRROR_CLASS(Derived2)
MIRROR_CLASS_MEMBER(derivedY)
MIRROR_CONSTRUCT_USING_MEMBER(derivedY) // #TODO Test constructing derived type
MIRROR_CLASS_END
MIRROR_TYPE(Derived2*)

MIRROR_CLASS(Base)
MIRROR_CLASS_MEMBER(baseX)
MirrorSubClassUserTypes<Base>(Mirror::MirrorTemplateArgumentList<Derived1, Derived2>{}, localStaticTypeInfo, 0);
MIRROR_CLASS_END
MIRROR_TYPE(Base*)

MIRROR_CLASS(TestStruct)
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
MIRROR_CLASS_END
MIRROR_TYPE(TestStruct*)
