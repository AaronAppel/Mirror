#include "Serialize.h"

#include <filesystem>
#include <stdint.h>
#include <string>
#include <unordered_map>

#include "../../Source/MIR_Mirror.h"
#include "../cJSON/cJSON.h"

#include "../Source/ForMirror.h"

namespace Serialize {

    bool Local_OverrideDeserializeForType(const cJSON* objectJson, const Mirror::TypeInfo* objectTypeInfo, void* objectAddress);

    void Local_DeserializePrimitive(const cJSON* const primitiveJson, const Mirror::TypeInfo* const primitiveTypeInfo, void* primitiveAddress);
    void Local_DeserializeClass(const cJSON* const classJson, const Mirror::TypeInfo* const classTypeInfo, void* classAddress);
    void Local_DeserializeCollection(const cJSON* const collectionJson, const Mirror::TypeInfo* const collectionTypeInfo, void* collectionAddress);
    void Local_DeserializePointer(const cJSON* const pointerJson, const Mirror::TypeInfo* const pointerTypeInfo, void* pointerAddress);

    void FromJson(const cJSON* objectJson, const Mirror::TypeInfo* const objectTypeInfo, void* objectAddress)
    {
        // ASSERT(objectAddress && objectTypeInfo && objectJson, "Null argument passed!");

        if (Local_OverrideDeserializeForType(objectJson, objectTypeInfo, objectAddress))
            return;

        switch (objectTypeInfo->category)
        {
        case Mirror::TypeInfoCategories::TypeInfoCategory_Primitive:
            Local_DeserializePrimitive(objectJson, objectTypeInfo, objectAddress); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Class:
            Local_DeserializeClass(objectJson, objectTypeInfo, objectAddress); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Collection:
            Local_DeserializeCollection(objectJson, objectTypeInfo, objectAddress); break;
        case Mirror::TypeInfoCategories::TypeInfoCategory_Pointer:
            Local_DeserializePointer(objectJson, objectTypeInfo, objectAddress); break;
        default:
            // ASSERT(false, "Invalid or unhandled objectTypeInfo->category!");
            break;
        }
    }

    void Local_DeserializePrimitive(const cJSON* const primitiveJson, const Mirror::TypeInfo* const primitiveTypeInfo, void* primitiveAddress)
    {
        // ASSERT(primitiveAddress && primitiveTypeInfo && primitiveJson, "Null argument passed!"); // #NOTE Don't compare category in case of override

        std::string* str = (std::string*)primitiveAddress;
        switch (primitiveTypeInfo->id)
        {
        case Mirror::IdForType<std::filesystem::path>():            {
            primitiveTypeInfo->typeConstructorFunc(primitiveAddress); // #NOTE String must be constructed before assigned to
            *(std::filesystem::path*)primitiveAddress = primitiveJson->valuestring ? primitiveJson->valuestring : primitiveJson->string; break;
            int bp = 0;
        }
            break;
        case Mirror::IdForType<const std::string>():
        case Mirror::IdForType<std::string>(): // #TODO Handles when obj is pair.first better. This will break if key and value are both string/char*
            {
                primitiveTypeInfo->typeConstructorFunc(primitiveAddress); // #NOTE String must be constructed before assigned to
                *(std::string*)primitiveAddress = primitiveJson->valuestring ? primitiveJson->valuestring : primitiveJson->string; break;
            }
        case Mirror::IdForType<const char*>():
        case Mirror::IdForType<char*>():
            *(const char**)primitiveAddress = _strdup(primitiveJson->valuestring); break;

        case Mirror::IdForType<const uint64_t>():
        case Mirror::IdForType<uint64_t>(): // #NOTE Storing uint64 as string
            *(uint64_t*)primitiveAddress = std::stoull(primitiveJson->valuestring); break;
        case Mirror::IdForType<const int64_t>():
        case Mirror::IdForType<int64_t>(): // #NOTE Storing int64 as string
            *(int64_t*)primitiveAddress = std::stoll(primitiveJson->valuestring); break;

        case Mirror::IdForType<const float>():
        case Mirror::IdForType<float>():
            *(float*)primitiveAddress = (float)primitiveJson->valuedouble; break;

        case Mirror::IdForType<const char>():
        case Mirror::IdForType<char>():
            memcpy(primitiveAddress, primitiveJson->valuestring, 1); break;

        default:
            if (primitiveJson->type | cJSON_Number | cJSON_True | cJSON_False)
            {   // #TODO Potential bug writing to fieldAddress with size less than primitiveJson->valueint
                void* fieldAddress = ((char*)primitiveAddress);
                void* sourceAddress = (void*)&primitiveJson->valueint;

                if (primitiveTypeInfo->size > 4)
                {
                    sourceAddress = (void*)&primitiveJson->valuedouble;
                }
                memcpy(fieldAddress, sourceAddress, primitiveTypeInfo->size);
            }
            else
            {
                // LOG_WARN("{0} Unsupported json object type!", __FUNCTION__);
            }
            break;
        }
    }

    void Local_DeserializeClass(const cJSON* const classJson, const Mirror::TypeInfo* const classTypeInfo, void* classAddress)
    {
        if (!classAddress || !classTypeInfo || !classJson || Mirror::TypeInfoCategory_Class != classTypeInfo->category)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        if (classTypeInfo->superTypeInfo)
        {
            Local_DeserializeClass(classJson, classTypeInfo->superTypeInfo, classAddress);
        }

        if (classTypeInfo->typeConstructorFunc)
        {
            classTypeInfo->typeConstructorFunc(classAddress);
        }

        if (classTypeInfo->fields.empty())
            return;

        const cJSON* iterator = classJson->child;
        while (iterator)
        {
            for (size_t i = 0; i < classTypeInfo->fields.size(); i++)
            {
                const Mirror::Field& field = classTypeInfo->fields[i];

                if (strcmp(field.name.c_str(), iterator->string) != 0 || field.flags & FieldSerializationFlags::_SkipDeserialize)
                    continue;

                void* fieldAddress = (char*)classAddress + field.offset;

                if (field.typeInfo->category == Mirror::TypeInfoCategory_Collection)
                {   // #NOTE Needed to pass name of member collection // #TODO Review and fix named collection member
                    Local_DeserializeCollection(iterator, field.typeInfo, fieldAddress);
                }
                else
                {
                    FromJson(iterator, field.typeInfo, (char*)classAddress + field.offset);
                }
                break;
            }

            iterator = iterator->next;
        }
    }

    void Local_DeserializeCollection(const cJSON* const collectionJson, const Mirror::TypeInfo* const collectionTypeInfo, void* collectionAddress)
    {
        if (!collectionAddress || !collectionTypeInfo || !collectionJson || Mirror::TypeInfoCategory_Collection != collectionTypeInfo->category)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        collectionTypeInfo->Construct(collectionAddress);

        size_t elementIndex = 0;
        const cJSON* jsonIterator = collectionJson->child;
        while (jsonIterator)
        {
            /* Testing tuple support
            {
                const Mirror::TypeInfo* currentTypeInfo = collectionTypeInfo->collectionTypeInfos[typeInfoIndex];

                Buffer elementFirstBuffer(currentTypeInfo->size);
                FromJson(jsonIterator, currentTypeInfo, elementFirstBuffer.As<void>());

                // collectionTypeInfo->CollectionAppend(collectionAddress, elementIndex, elementFirstBuffer.As<void>(), elementSecondBuffer.As<void>());

                if (typeInfoIndex + 1 < collectionTypeInfo->collectionTypeInfos.size())
                {
                    ++typeInfoIndex;
                }
                else
                {
                    break; // #TODO Review. Shouldn't need to break. End on vector last element, or tuple last type. Both should be jsonIterator->next == nullptr
                }
                continue;
            }
            //*/

            // Old
            Buffer elementFirstBuffer(collectionTypeInfo->collectionTypeInfoFirst->size);
            FromJson(jsonIterator, collectionTypeInfo->collectionTypeInfoFirst, elementFirstBuffer.As<void>());

            const bool isPair = collectionTypeInfo->collectionTypeInfoSecond;
            Buffer elementSecondBuffer(isPair ? collectionTypeInfo->collectionTypeInfoSecond->size : 0);
            if (isPair)
            {
                FromJson(jsonIterator->next, collectionTypeInfo->collectionTypeInfoSecond, elementSecondBuffer.As<void>());
            }
            collectionTypeInfo->CollectionAppend(collectionAddress, elementIndex, elementFirstBuffer.As<void>(), elementSecondBuffer.As<void>());

            if (isPair)
            {
                break; // #NOTE 1 iteration for pairs #TODO Review tuples
            }
            jsonIterator = jsonIterator->next;
            ++elementIndex;
        }
        return;
    }

    void Local_DeserializePointer(const cJSON* const pointerJson, const Mirror::TypeInfo* const pointerTypeInfo, void* pointerAddress)
    {
        if (!pointerAddress || !pointerTypeInfo || !pointerJson || Mirror::TypeInfoCategory_Pointer != pointerTypeInfo->category)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return;
        }

        if (strcmp(pointerJson->child->string, "nullptr") == 0)
        {
            // LOG_WARN("{0} Null pointer!", __FUNCTION__);
            *(void**)pointerAddress = nullptr;
            return;
        }

        const Mirror::TypeInfo* dereferencedTypeInfo = pointerTypeInfo->pointerDereferencedTypeInfo->DerivedTypeFromTypeName(pointerJson->child->string);

        void* derefencedTypeObjAddress = new char[dereferencedTypeInfo->size];
        *(void**)pointerAddress = derefencedTypeObjAddress;

        FromJson(pointerJson->child, dereferencedTypeInfo, derefencedTypeObjAddress);
    }

    bool Local_OverrideDeserializeForType(const cJSON* objectJson, const Mirror::TypeInfo* objectTypeInfo, void* obj)
    {
        if (!obj || !objectTypeInfo || !objectJson)
        {
            // LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
            return false;
        }

        switch (objectTypeInfo->id)
        {
        // #NOTE Treat some types as primitives
        // case Mirror::IdForType<GUID>(): // #TODO Look at treating a GUID as a primitive type, and use owner member name before m_Guid class member name
        case Mirror::IdForType<std::filesystem::path>():
        case Mirror::IdForType<std::string>():
        case Mirror::IdForType<const char*>():
            Local_DeserializePrimitive(objectJson, objectTypeInfo, obj);
            return true;
        }
        return false;
    }

}
