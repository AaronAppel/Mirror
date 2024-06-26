#include "Serialize.h"

namespace Serialize {

    void local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
    void local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson);
    void local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name);
    void local_SerializePair(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson);

    cJSON* CreateJsonObject(const char* key = nullptr);

    template <typename T>
    cJSON* CreateJsonBool(const std::string& name, const void* obj);
    template <typename T>
    cJSON* CreateJsonNumber(const std::string& name, const void* obj);
    template <typename T>
    cJSON* CreateJsonString(const std::string& name, const void* obj);

    bool local_TypeInfoHasOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson);

    void ToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name)
    {
        if (!obj || !objTypeInfo || !objJson)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        if (local_TypeInfoHasOverride(obj, objTypeInfo, objJson))
            return;

        switch (objTypeInfo->category)
        {
        case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
            local_SerializePrimitive(obj, objTypeInfo, objJson, objTypeInfo->stringName); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
            {
                cJSON* classJson = CreateJsonObject(name.c_str());
                cJSON_AddItemToArray(objJson, classJson);
                local_SerializeClass(obj, objTypeInfo, classJson);
            }
            break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
            local_SerializeCollection(obj, objTypeInfo, objJson, name); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
            {
                cJSON* pointerJson = CreateJsonObject(name.c_str());
                cJSON_AddItemToArray(objJson, pointerJson);

                if (nullptr == *(const void**)obj)
                {
                    // LOG_ERROR("{0} Pointer is null!", __FUNCTION__);
                    uint8_t pointerValue = 0;
                    local_SerializePrimitive(&pointerValue, Mirror::InfoForType<uint8_t>(), pointerJson, MIRROR_TO_STR(nullptr));
                    return;
                }

                const Mirror::TypeInfo* absoluteTypeInfo = objTypeInfo->AbsoluteType()->DerivedTypeFromPointer(obj);
                ToJson(*(void**)obj, absoluteTypeInfo, pointerJson, absoluteTypeInfo->stringName);
            }
            break;

        case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
            local_SerializePair(obj, objTypeInfo, objJson); break;
        }
    }

    void local_SerializePrimitive(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
    {
        if (!obj || !objTypeInfo || !objJson)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        cJSON* cJsonItem = nullptr;

        switch (objTypeInfo->id)
        {
        case Mirror::TypeId<std::string>():
            {
                auto result = Mirror::TypeId<std::string>();
                const std::string* fieldAddress = (std::string*)obj;
                cJsonItem = CreateJsonString<const char>(name, fieldAddress->data()); // #TODO Requires ->data() so can't work with CreateJsonString()
            }
            break;

        case Mirror::TypeId<char>():
            {
                auto result = Mirror::TypeId<char>();
                char charArr[2] = { '\0', '\0' };
                charArr[0] = *(char*)obj;
                cJsonItem = cJSON_CreateString(charArr);
                cJsonItem->string = _strdup(name.c_str());
            }
            break;

        case Mirror::TypeId<char*>():
        case Mirror::TypeId<const char*>():
            cJsonItem = CreateJsonString<const char>(name, *(void**)obj); break;

        case Mirror::TypeId<bool>():
            cJsonItem = CreateJsonBool<bool>(name, obj); break;

        case Mirror::TypeId<uint8_t>():
            cJsonItem = CreateJsonBool<uint8_t>(name, obj); break;

        case Mirror::TypeId<uint16_t>():
            cJsonItem = CreateJsonBool<uint16_t>(name, obj); break;

        case Mirror::TypeId<uint32_t>():
            cJsonItem = CreateJsonBool<uint32_t>(name, obj); break;

        case Mirror::TypeId<int8_t>():
            cJsonItem = CreateJsonBool<int8_t>(name, obj); break;

        case Mirror::TypeId<int16_t>():
            cJsonItem = CreateJsonBool<int16_t>(name, obj); break;

        case Mirror::TypeId<int32_t>():
            cJsonItem = CreateJsonBool<int32_t>(name, obj); break;

        case Mirror::TypeId<int64_t>(): // #NOTE Special case of conversion on 64 bit types
            {
                // Use string instead of a double to avoid conversion issues
                int64_t* numberAddress = (int64_t*)obj;
                cJsonItem = CreateJsonString<const char>(name, std::to_string(*numberAddress).c_str());
                // #TODO Try using objJson->valuedouble and a memcpy to see if that works
            }
            break;

        case Mirror::TypeId<uint64_t>(): // #NOTE Special case of conversion on 64 bit types
        {
            // Use string instead of a double to avoid conversion issues
            uint64_t* numberAddress = (uint64_t*)obj;
            cJsonItem = CreateJsonString<const char>(name, std::to_string(*numberAddress).c_str());
            // #TODO Try using objJson->valuedouble and a memcpy to see if that works
        }
        break;

        case Mirror::TypeId<float>():
            cJsonItem = CreateJsonNumber<float>(name, obj); break;

        case Mirror::TypeId<double>():
            cJsonItem = CreateJsonNumber<double>(name, obj); break;

        default:
            // LOG_ERROR("{0} Unsupported user defined field type {1} {2}({3}) for serialization!", __FUNCTION__, name.c_str(), objTypeInfo->stringName.c_str(), (int)objTypeInfo->enumType);
            break;
        }

        if (cJsonItem)
        {
            cJSON_AddItemToArray(objJson, cJsonItem);
        }
    }

    void local_SerializeClass(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson)
    {
        if (!obj || !objTypeInfo || !objJson || objTypeInfo->category != Mirror::TypeInfoCategory_Class)
        {
            // LOG_ERROR("{0} Invalid argument passed!", __FUNCTION__);
            return;
        }

        if (objTypeInfo->superTypeInfo)
        {
            local_SerializeClass(obj, objTypeInfo->superTypeInfo, objJson);
        }

        for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
        {
            const Mirror::Field& field = objTypeInfo->fields[i];

            // if (field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
            //     continue;

            const void* fieldAddress = (char*)obj + field.offset;

            // #TODO No if check needed?
            if (Mirror::TypeInfoCategory_Primitive == field.typeInfo->category)
            {   // #TODO Review handling primitive here (dependent on field name)
                local_SerializePrimitive(fieldAddress, field.typeInfo, objJson, field.name);
            }
            else
            {   // #TODO This also checks if primitive so maybe avoid redundancy
                ToJson(fieldAddress, field.typeInfo, objJson, field.name);
            }
        }
    }

    void local_SerializeCollection(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson, const std::string& name)
    {
        if (!obj || !objTypeInfo || !objJson || Mirror::TypeInfoCategory_Collection != objTypeInfo->category)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        if (false) // (!objTypeInfo->newIsCollection()) // || !objTypeInfo->collectionTypeInfo)
        {
            // LOG_ERROR("{0} Invalid type info!", __FUNCTION__);
            return;
        }

        cJSON* collectionJsonContainer = CreateJsonObject(name.c_str());
        cJSON_AddItemToArray(objJson, collectionJsonContainer);

        size_t counter = 0;
        void* elementAddress = (void*)objTypeInfo->collectionIterateCurrentFunc(obj, counter);
        while (elementAddress)
        {
            if (auto firstTypeInfo = objTypeInfo->collectionTypeInfoFirst)
            {
                ToJson(elementAddress, firstTypeInfo, collectionJsonContainer, firstTypeInfo->stringName);
            }
            else
            {
                // LOG_WARN("{0} First info is null!", __FUNCTION__);
            }
            ++counter;
            elementAddress = (void*)objTypeInfo->collectionIterateCurrentFunc(obj, counter);
        }
    }

    void local_SerializePair(const void* const obj, const Mirror::TypeInfo* const objTypeInfo, cJSON* objJson)
    {
        cJSON* pairJson = CreateJsonObject(objTypeInfo->stringName.c_str());
        cJSON_AddItemToArray(objJson, pairJson);

        const Mirror::TypeInfo* const objTypeInfoFirst = objTypeInfo->collectionTypeInfoFirst;
        const Mirror::TypeInfo* const objTypeInfoSecond = objTypeInfo->collectionTypeInfoSecond;

        const void* const firstAddress = objTypeInfo->collectionAddressOfPairObjectFunc(obj, true);
        const void* const secondAddress = objTypeInfo->collectionAddressOfPairObjectFunc(obj, false);

        if (objTypeInfo->collectionTypeInfoFirst->category == Mirror::TypeInfoCategory_Primitive)
        {
            cJSON* pairFirstJson = CreateJsonObject(objTypeInfoFirst->stringName.c_str());
            cJSON_AddItemToArray(pairJson, pairFirstJson);
            local_SerializePrimitive(firstAddress, objTypeInfoFirst, pairFirstJson, objTypeInfoFirst->stringName);
        }
        else
        {
            ToJson(firstAddress, objTypeInfoFirst, pairJson, objTypeInfoFirst->stringName);
        }

        cJSON* pairSecondJson = CreateJsonObject(objTypeInfoSecond->stringName.c_str());
        if (Mirror::TypeInfoCategories::TypeInfoCategory_Collection != objTypeInfoSecond->category)
        {
            cJSON_AddItemToArray(pairJson, pairSecondJson);
        }

        switch (objTypeInfoSecond->category)
        {
        case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
            local_SerializePrimitive(secondAddress, objTypeInfoSecond, pairSecondJson, objTypeInfoSecond->stringName); break;

        case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
            local_SerializeClass(secondAddress, objTypeInfoSecond, pairSecondJson); break;

        case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
            // #NOTE Can't add because collection already adds
            local_SerializeCollection(secondAddress, objTypeInfoSecond, pairJson, objTypeInfoSecond->stringName); break;

        case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
        {
            const Mirror::TypeInfo* secondAbsoluteTypeInfoDerived = objTypeInfoSecond->AbsoluteType()->DerivedTypeFromPointer(secondAddress);
            ToJson(*(void**)secondAddress, secondAbsoluteTypeInfoDerived, pairSecondJson, secondAbsoluteTypeInfoDerived->stringName);
        }
        break;

        case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
            local_SerializePair(secondAddress, objTypeInfoSecond, pairSecondJson); break;
        }
    }

    cJSON* CreateJsonObject(const char* key)
    {
        cJSON* returnJSONItem = cJSON_CreateObject();
        if (key) returnJSONItem->string = _strdup(key); // #TODO Look into deallocation responsibility
        return returnJSONItem;
    }

    template <typename T>
    cJSON* CreateJsonBool(const std::string& name, const void* obj)
    {
        cJSON* returnBool = cJSON_CreateBool(*(T*)obj);
        returnBool->string = _strdup(name.c_str()); // #TODO Check memory deallocation
        return returnBool;
    }

    template <typename T>
    cJSON* CreateJsonNumber(const std::string& name, const void* obj)
    {
        cJSON* returnNumber = cJSON_CreateNumber(*(T*)obj);
        returnNumber->string = _strdup(name.c_str());
        return returnNumber;
    }

    template <typename T>
    cJSON* CreateJsonString(const std::string& name, const void* obj)
    {
        cJSON* returnString = cJSON_CreateString((T*)obj); // #NOTE (T*)obj is not dereferenced
        returnString->string = _strdup(name.c_str());
        return returnString;
    }

    bool local_TypeInfoHasOverride(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson)
    {
        switch (objTypeInfo->id)
        {
        case 0: // Avoid empty switch warning
        default:
            break;
        }
        return false;
    }

}
