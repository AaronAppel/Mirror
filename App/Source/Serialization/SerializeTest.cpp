
#include "Mirror.h"
#include "MirrorTesting.h"

#include "Serialize.h"

void SerializeTest()
{
    // Serialize with unique values
    TestStruct testStructSerialize;
    auto loc = &testStructSerialize.m_UmapStringInt32;

    testStructSerialize.m_Base.baseX = 90;
    testStructSerialize.m_Derived1.derivedZ = 80;
    testStructSerialize.m_Derived1.baseX = 70;
    if (testStructSerialize.m_Derived1Ptr)
    {
        testStructSerialize.m_Derived1Ptr->baseX = 60;
        testStructSerialize.m_Derived1Ptr->derivedZ = 50;
    }
    for (size_t i = 0; i < 10; i++)
    {
        testStructSerialize.m_FloatArray10[i] = i + (.1 * i);
    }
    for (size_t i = 0; i < 5; i++)
    {
        testStructSerialize.m_CharVector.push_back(66 + i);
    }
    testStructSerialize.m_Bool = !testStructSerialize.m_Bool;
    testStructSerialize.m_U8 += 1;
    testStructSerialize.m_U16 += 1;
    testStructSerialize.m_U32 += 1;
    testStructSerialize.m_U64 += 1;
    testStructSerialize.m_S8 += 1;
    testStructSerialize.m_S16 += 1;
    testStructSerialize.m_S32 += 1;
    testStructSerialize.m_S64 += 1;
    testStructSerialize.m_Float += 1.5;
    testStructSerialize.m_Double += 1.5;
    testStructSerialize.m_String = "test";
    testStructSerialize.m_ConstCharPtr = "test";
    testStructSerialize.m_UmapStringInt32.clear();
    testStructSerialize.m_UmapStringInt32.insert({ "test", 890 });
    auto loc2 = &testStructSerialize.m_UmapStringInt32;

    testStructSerialize.m_Derived1Ptr = new Derived1();
    testStructSerialize.m_Derived2Ptr = new Derived2();

    if (true) testStructSerialize.m_BasePtrDerived = new Derived1();
    else if (true) testStructSerialize.m_BasePtrDerived = new Derived2();
    else testStructSerialize.m_BasePtrDerived = new Base();

    Serialize::ToFile(testStructSerialize, "TestStruct");

    // Deserialize
    TestStruct testStructDeserialize;
    Serialize::FromFile("TestStruct", testStructDeserialize);
    testStructDeserialize.m_BasePtrDerived;
}

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
MIRROR_CLASS_END(TestStruct);

