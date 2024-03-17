#include "Serialization.h"

#include <basetsd.h> // INT32, INT64
#include <string.h> // _strdup()
#include <vector>

#include "../../cJSON/Source/cJSON.h"

#include "../main.h"

// Just some old, convenient helpers
std::vector<cJSON*> GetAllItemsFromArray(const cJSON* arrayObject);
cJSON* GetItemFromArrayByIndex(const cJSON* cJSONarray, int index);
void AddItemToArray(cJSON* cJSONArray, cJSON* item);

namespace Serialization
{
#if _WIN64 // #TODO Review platform pointer sizes
    typedef PINT64 PlatformPointer;
#elif _WIN32
    typedef PINT32 PlatformPointer;
#endif

    // void WriteAny(const void* source, const Field& field, void* target)
    // {
    //     memcpy((char*)target + field.offset, source, field.type->size); // #TODO Source and target size mismatch. Could pick smaller value
    // }

    void DeserializeJsonString(const cJSON* jsonObj, const Mirror::Field& field, void* obj)
    {
        if (!jsonObj || !jsonObj->valuestring || (jsonObj->type != cJSON_String) || !obj)
            return;

        switch (field.typeInfo->enumType)
        {
            case MirrorTypes::m_string:
            {
                size_t smallerSize = strlen(jsonObj->valuestring);

                std::string* fieldAddress = reinterpret_cast<std::string*>((char*)obj + field.offset);
                if (smallerSize > fieldAddress->size())
                {
                    smallerSize = field.typeInfo->size;
                }

                if (smallerSize > 0)
                {
                    const void* sourceAddr = (const void*)jsonObj->valuestring;
                    memcpy((void*)fieldAddress->data(), sourceAddr, smallerSize);
                }
            }
            break;

            case MirrorTypes::m_char:
            {
                jsonObj->valuestring;
                void* fieldAddress = (char*)obj + field.offset;
                memcpy(fieldAddress, jsonObj->valuestring, sizeof(void*));
            }
            break;

            case MirrorTypes::m_charPtr:
            case MirrorTypes::m_constCharPtr:
            {
                PlatformPointer* writeAddress = (PlatformPointer*)((char*)obj + field.offset);
                *writeAddress = (PlatformPointer)_strdup(jsonObj->valuestring);
            }
            break;
        }
    }

    void DeserializeJsonNumber(const cJSON* jsonObj, const Mirror::Field& field, void* obj)
    {
        if (!jsonObj || (jsonObj->type == cJSON_String) || (jsonObj->type == cJSON_NULL) || !obj)
            return;

        switch (field.typeInfo->enumType)
        {
            case MirrorTypes::m_int8_t:
            case MirrorTypes::m_int16_t:
            case MirrorTypes::m_int32_t:
            case MirrorTypes::m_int64_t:
            case MirrorTypes::m_uint8_t:
            case MirrorTypes::m_uint16_t:
            case MirrorTypes::m_uint32_t:
            case MirrorTypes::m_uint64_t:
            case MirrorTypes::m_int:
            case MirrorTypes::m_bool:
            case MirrorTypes::m_float:
            case MirrorTypes::m_double:
            {
                size_t smallerSize = sizeof(jsonObj->valueint);
                if (smallerSize > field.typeInfo->size)
                {
                    smallerSize = field.typeInfo->size;
                }

                if (smallerSize > 0)
                {
                    void* fieldAddress = (char*)obj + field.offset;
                    const void* sourceAddr = (const void*)&jsonObj->valueint;
                    memcpy(fieldAddress, sourceAddr, smallerSize);
                }
            }
            break;
        }
    }

    void DeserializeJsonPrimitive(const cJSON* jsonObj, const Mirror::TypeInfo* TypeInfo, void* obj)
    {
        if (!jsonObj || !obj)
            return;

        switch (jsonObj->type)
        {
        case cJSON_String:
            {
                Mirror::Field field;
                DeserializeJsonString(jsonObj, field, obj);
            }
            break;

        case cJSON_True:
        case cJSON_False:
        case cJSON_NULL:
        case cJSON_Number:
        case cJSON_Array:
        case cJSON_Object:
            {
                Mirror::Field field;
                DeserializeJsonNumber(jsonObj, field, obj);
            }
            break;
        }
    }

    void DeserializeJsonObject(const cJSON* jsonObj, const Mirror::TypeInfo* TypeInfo, void* obj)
    {
        if (!jsonObj || !obj)
            return;

        if (!TypeInfo)
        {
            // Try to handle the case where an int was passed instead of a class
            DeserializeJsonPrimitive(jsonObj, TypeInfo, obj);
            return;
        }

        const std::vector<cJSON*> arr = GetAllItemsFromArray(jsonObj);
        for (size_t i = 0; i < arr.size(); i++)
        {
            const cJSON* arrItem = arr[i];
            if (!arrItem)
                continue;

            for (size_t i = 0; i < TypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = TypeInfo->fields[i];

                if (!field.typeInfo)
                    break; // #TODO Review #NOTE Break early if fields become null (and future values are null) to save iterations

                if (strcmp(field.name.c_str(), arrItem->string) != 0) // #NOTE This logic matches the type name, but can be extended
                    continue;

                switch (arrItem->type)
                {
                    case cJSON_String:
                        DeserializeJsonString(arrItem, field, obj);
                        break;

                    case cJSON_True:
                    case cJSON_False:
                    case cJSON_NULL:
                    case cJSON_Number:
                    case cJSON_Array:
                    case cJSON_Object:
                        DeserializeJsonNumber(arrItem, field, obj);
                        break;
                }
            }
        }
    }

    void SerializeJsonObject(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* jsonObj)
    {
        if (!jsonObj || !objTypeInfo || !obj)
            return;

        for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
        {
            const Mirror::Field& field = objTypeInfo->fields[i];

            if (!field.typeInfo)
                break; // #NOTE Future fields should also be null

            switch (field.typeInfo->enumType)
            {
            case MirrorTypes::ExampleStruct:
                {
                    SerializeJsonObject(obj, Mirror::InfoForType<ExampleStruct>(), jsonObj);
                }
                break;

            case MirrorTypes::ExampleClass:
                {
                    SerializeJsonObject(obj, Mirror::InfoForType<ExampleClass>(), jsonObj);
                }
                break;

            case MirrorTypes::ExampleNestedCutomTypes:
                {
                    cJSON* arr = cJSON_CreateArray();
                    arr->string = _strdup(Mirror::InfoForType<ExampleNestedCutomTypes>()->stringName.c_str());
                    cJSON_AddItemToArray(jsonObj, arr);
                    SerializeJsonObject(obj, Mirror::InfoForType<ExampleNestedCutomTypes>(), arr);
                }
                break;

            default:
                SerializeJsonType(obj, field, jsonObj);
                break;
            }
        }
    }

    void SerializeJsonType(const void* obj, const Mirror::Field& field, cJSON* jsonObj)
    {
        if (!jsonObj || !obj)
            return;

        switch (field.typeInfo->enumType)
        {
        case MirrorTypes::m_string:
            {
                const std::string* fieldAddress = (const std::string*)((char*)obj + field.offset);
                cJSON* newcJsonStr = cJSON_CreateString(fieldAddress->c_str());
                newcJsonStr->string = _strdup(field.name.c_str());
                AddItemToArray(jsonObj, newcJsonStr);
            }
            break;

        case MirrorTypes::m_charPtr:
        case MirrorTypes::m_constCharPtr:
            {
                const char* fieldAddress = *(const char**)((char*)obj + field.offset);
                cJSON* newcJsonStr = cJSON_CreateString(fieldAddress);
                newcJsonStr->string = _strdup(field.name.c_str());
                AddItemToArray(jsonObj, newcJsonStr);
            }
            break;

        case MirrorTypes::m_char:
            {
                char* fieldAddress = (char*)obj + field.offset;
                char nullTerminatedString[2] = "\0";
                nullTerminatedString[0] = *fieldAddress;
                cJSON* newcJsonStr = cJSON_CreateString(nullTerminatedString);
                newcJsonStr->string = _strdup(field.name.c_str());
                AddItemToArray(jsonObj, newcJsonStr);
            }
            break;

        case MirrorTypes::m_bool:
            {
                bool* boolAddress = (bool*)((char*)obj + field.offset);
                cJSON* newcJsonBool = cJSON_CreateBool(*boolAddress);
                newcJsonBool->string = _strdup(field.name.c_str());
                AddItemToArray(jsonObj, newcJsonBool);
            }
            break;

        case MirrorTypes::m_int8_t:
        case MirrorTypes::m_int16_t:
        case MirrorTypes::m_int32_t:
        case MirrorTypes::m_int64_t: // #TODO Handle different number sizes properly
        case MirrorTypes::m_uint8_t:
        case MirrorTypes::m_uint16_t:
        case MirrorTypes::m_uint32_t:
        case MirrorTypes::m_uint64_t:
        case MirrorTypes::m_int:
        case MirrorTypes::m_float:
        case MirrorTypes::m_double:
            {
                int* numberAddress = (int*)((char*)obj + field.offset);
                cJSON* newcJsonNumber = cJSON_CreateNumber(*numberAddress);
                newcJsonNumber->string = _strdup(field.name.c_str());
                AddItemToArray(jsonObj, newcJsonNumber);
            }
            break;

        default:
            break;
        }
    }

    void SerializeJsonType(const void* obj, const Mirror::TypeInfo& typeInfo, cJSON* jsonObj)
    {
        if (!obj || !jsonObj)
            return;

        switch (typeInfo.enumType)
        {
        case MirrorTypes::m_string:
        {
            const std::string* fieldAddress = (const std::string*)obj;
            cJSON* newcJsonStr = cJSON_CreateString(fieldAddress->c_str());
            newcJsonStr->string = _strdup(typeInfo.stringName.c_str());
            AddItemToArray(jsonObj, newcJsonStr);
        }
        break;

        case MirrorTypes::m_charPtr:
        case MirrorTypes::m_constCharPtr:
        {
            const char* fieldAddress = *(const char**)obj;
            cJSON* newcJsonStr = cJSON_CreateString(fieldAddress);
            newcJsonStr->string = _strdup(typeInfo.stringName.c_str());
            AddItemToArray(jsonObj, newcJsonStr);
        }
        break;

        case MirrorTypes::m_char:
        {
            char* fieldAddress = (char*)obj;
            char nullTerminatedString[2] = "\0";
            nullTerminatedString[0] = *fieldAddress;
            cJSON* newcJsonStr = cJSON_CreateString(nullTerminatedString);
            newcJsonStr->string = _strdup(typeInfo.stringName.c_str());
            AddItemToArray(jsonObj, newcJsonStr);
        }
        break;

        case MirrorTypes::m_bool:
        {
            bool* boolAddress = (bool*)obj;
            cJSON* newcJsonBool = cJSON_CreateBool(*boolAddress);
            newcJsonBool->string = _strdup(typeInfo.stringName.c_str());
            AddItemToArray(jsonObj, newcJsonBool);
        }
        break;

        case MirrorTypes::m_int8_t:
        case MirrorTypes::m_int16_t:
        case MirrorTypes::m_int32_t:
        case MirrorTypes::m_int64_t: // #TODO Handle different number sizes properly
        case MirrorTypes::m_uint8_t:
        case MirrorTypes::m_uint16_t:
        case MirrorTypes::m_uint32_t:
        case MirrorTypes::m_uint64_t:
        case MirrorTypes::m_int:
        case MirrorTypes::m_float:
        case MirrorTypes::m_double:
        {
            int* numberAddress = (int*)obj;
            cJSON* newcJsonNumber = cJSON_CreateNumber(*numberAddress);
            newcJsonNumber->string = _strdup(typeInfo.stringName.c_str());
            AddItemToArray(jsonObj, newcJsonNumber);
        }
        break;

        default:
            break;
        }

        // memcpy(jsonObj, obj, typeInfo.size);
    }

}

std::vector<cJSON*> GetAllItemsFromArray(const cJSON* arrayObject)
{
    std::vector<cJSON*> itemList;

    if (arrayObject->child == nullptr)
        return itemList;

    const int t_ArraySize = cJSON_GetArraySize(arrayObject->child);
    itemList.reserve(t_ArraySize);
    for (int i = 0; i < t_ArraySize; i++)
    {
        itemList.push_back(cJSON_GetArrayItem(arrayObject->child, i));
    }

    return itemList;
}

cJSON* GetItemFromArrayByIndex(const cJSON* cJSONarray, int index)
{
    cJSON* cJSONItem = cJSONarray->child->child;

    for (int i = 0; i < index; i++)
    {
        cJSONItem = cJSONItem->next;
    }
    return cJSONItem;
}

void AddItemToArray(cJSON* cJSONArray, cJSON* item)
{
    int arraySize = 0;

    if (cJSONArray->child)
    {
        arraySize = cJSON_GetArraySize(cJSONArray->child);
    }
    else
    {
        cJSONArray->child = cJSON_CreateObject();
    }

    if (arraySize > 0)
    {
        cJSON* lastSibling = GetItemFromArrayByIndex(cJSONArray, arraySize - 1);
        lastSibling->next = item;
        item->prev = lastSibling;
    }
    else
    {
        if (cJSONArray->child->child == nullptr)
        {
            cJSONArray->child->child = item;
        }
    }
}
