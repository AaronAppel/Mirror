#include "Serialize.h"

namespace Serialize {

    void local_DeserializePrimitive(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool useNameString = false);
    void local_DeserializeClass(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj);
    void local_DeserializeCollection(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);
    void local_DeserializePair(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name);

    bool local_TypeInfoHasOverride(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, const void* obj);

    void FromJson(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
    {
        if (!obj || !objTypeInfo || !objJson)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        if (local_TypeInfoHasOverride(objJson, objTypeInfo, obj))
            return;

        switch (objTypeInfo->category)
        {
        case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
            local_DeserializePrimitive(objJson, objTypeInfo, obj); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
            local_DeserializeClass(objJson, objTypeInfo, obj); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
            local_DeserializeCollection(objJson, objTypeInfo, obj, objTypeInfo->stringName); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
            {
                if (strcmp(objJson->child->string, MIRROR_TO_STR(nullptr)) == 0)
                {
                    // LOG_WARN("{0} Null pointer!", __FUNCTION__);
                    *(void**)obj = nullptr;
                    return;
                }

                const Mirror::TypeInfo* dereferencedTypeInfo = objTypeInfo->pointerDereferencedTypeInfo->DerivedTypeFromTypeName(objJson->child->string);

                void* derefencedTypeObjAddress = *(void**)obj;
                derefencedTypeObjAddress = new char[dereferencedTypeInfo->size];
                *(void**)obj = derefencedTypeObjAddress;

                if (dereferencedTypeInfo->typeConstructorFunc)
                {   // #TODO Only serialize dependent member(s)?
                    FromJson(objJson->child, dereferencedTypeInfo, derefencedTypeObjAddress);
                }
                dereferencedTypeInfo->Construct(derefencedTypeObjAddress);

                FromJson(objJson->child, dereferencedTypeInfo, derefencedTypeObjAddress);
            }
            break;

        case Mirror::TypeInfoCategories::TypeInfoCategory_Pair:
            local_DeserializePair(objJson, objTypeInfo, obj, objTypeInfo->stringName); break;
        }
    }

    void local_DeserializePrimitive(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, bool isPairFirst)
    {
        if (!obj || !objTypeInfo || !objJson)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        switch (objTypeInfo->id)
        {
        case Mirror::TypeId<std::string>():
            // #TODO Handles when obj is pair.first better. This will break if key and value are both string/char*
            objTypeInfo->typeConstructorFunc(obj); // #NOTE String must be constructed before assigned to
            *(std::string*)obj = objJson->valuestring ? objJson->valuestring : objJson->string;
            break;

        case Mirror::TypeId<char>():
            memcpy(obj, objJson->valuestring, 1); break;

        case Mirror::TypeId<char*>():
        case Mirror::TypeId<const char*>():
            *(const char**)obj = _strdup(objJson->valuestring); break;

        case Mirror::TypeId<int64_t>(): // #NOTE Storing uint64 as string
            *(int64_t*)obj = std::stoll(objJson->valuestring); break;

        case Mirror::TypeId<uint64_t>(): // #NOTE Storing uint64 as string
            *(uint64_t*)obj = std::stoull(objJson->valuestring); break;

        case Mirror::TypeId<float>():
            *(float*)obj = (float)objJson->valuedouble; break;

        default:
            if (objJson->type | cJSON_Number | cJSON_True | cJSON_False)
            {   // #TODO Potential bug writing to fieldAddress with size less than objJson->valueint
                void* fieldAddress = ((char*)obj);
                void* sourceAddress = (void*)&objJson->valueint;

                if (objTypeInfo->size > 4)
                {
                    sourceAddress = (void*)&objJson->valuedouble;
                }
                memcpy(fieldAddress, sourceAddress, objTypeInfo->size);
            }
            else
            {
                // LOG_WARN("{0} Unsupported json object type!", __FUNCTION__);
            }
            break;
        }
    }

    void local_DeserializeClass(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj)
    {
        if (objTypeInfo->superTypeInfo)
        {
            // #TODO Should super constructor be called?
            local_DeserializeClass(objJson, objTypeInfo->superTypeInfo, obj);
        }

        if (objTypeInfo->typeConstructorFunc)
        {
            objTypeInfo->typeConstructorFunc(obj);
        }

        if (objTypeInfo->fields.empty())
            return;

        const cJSON* iterator = objJson->child;
        while (iterator)
        {
            for (size_t i = 0; i < objTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = objTypeInfo->fields[i];

                if (strcmp(field.name.c_str(), iterator->string) != 0 )
                    // || field.serializationFlags & Mirror::FieldSerializationFlags::_InspectorOnly)
                    continue;

                void* fieldAddress = (char*)obj + field.offset;

                if (field.typeInfo->category == Mirror::TypeInfoCategory_Collection)
                {   // #NOTE Needed to pass name of member collection
                    local_DeserializeCollection(iterator, field.typeInfo, fieldAddress, field.name);
                }
                else
                {
                    FromJson(iterator, field.typeInfo, (char*)obj + field.offset);
                }
                break;
            }

            iterator = iterator->next;
        }
    }

    void local_DeserializeCollection(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name)
    {
        if (!obj || !objTypeInfo || !objJson)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->collectionTypeInfoFirst;
        char* elementFirstBuffer = new char[elementFirstTypeInfo->size];

        const cJSON* iterator = objJson->child;
        size_t index = 0; // #TODO Scrap index for non-contiguous collections if possible

        // #NOTE Collection constructor should be called
        objTypeInfo->Construct(obj);

        while (iterator)
        {
            FromJson(iterator, elementFirstTypeInfo, (void*)elementFirstBuffer);
            objTypeInfo->CollectionAppend(obj, index, (void*)elementFirstBuffer, nullptr);
            iterator = iterator->next;
            ++index;
        }
        delete[] elementFirstBuffer;
    }

    void local_DeserializePair(const cJSON* const objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj, const std::string& name)
    {
        if (!obj || !objTypeInfo || !objJson || objTypeInfo->category != Mirror::TypeInfoCategory_Pair)
        {
            // LOG_ERROR("{0} Invalid argument passed!", __FUNCTION__);
            return;
        }

        const Mirror::TypeInfo* const elementFirstTypeInfo = objTypeInfo->collectionTypeInfoFirst;
        char* elementFirstBuffer = new char[elementFirstTypeInfo->size];
        const cJSON* firstJson = objJson->child;
        if (Mirror::TypeInfoCategory_Primitive == elementFirstTypeInfo->category)
        {   // #TODO Review handling primitives differently
            firstJson = firstJson->child; // #NOTE Won't be wrapped in another cJSON object (like a class, collection, etc would)
        }
        FromJson(firstJson, elementFirstTypeInfo, (void*)elementFirstBuffer);

        const Mirror::TypeInfo* elementSecondInfo = objTypeInfo->collectionTypeInfoSecond;
        char* elementSecondBuffer = new char[elementSecondInfo ? elementSecondInfo->size : 0];
        const cJSON* secondJson = objJson->child->next;
        if (Mirror::TypeInfoCategory_Primitive == elementSecondInfo->category)
        {   // #TODO Review handling primitives differently
            secondJson = secondJson->child; // #NOTE Won't be wrapped in another cJSON object (like a class, collection, etc would)
        }
        FromJson(secondJson, elementSecondInfo, (void*)elementSecondBuffer);

        objTypeInfo->Construct(obj);
        objTypeInfo->CollectionAppend(obj, 0, (void*)elementFirstBuffer, (void*)elementSecondBuffer);

        delete[] elementFirstBuffer;
        delete[] elementSecondBuffer;
    }

    bool local_TypeInfoHasOverride(const cJSON* objJson, const Mirror::TypeInfo* objTypeInfo, const void* obj)
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
