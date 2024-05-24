
#include "../../cJSON/Source/cJSON.h"

#include "Mirror.h"

#include "Utilities.h"

struct cJSON;

namespace Serialize {

    void ToJson(const void* obj, const Mirror::TypeInfo* objTypeInfo, cJSON* objJson, const std::string& name);

    template <class T>
    void ToFile(const T& objectReference, const char* absoluteFilePath)
    {
        if (!absoluteFilePath)
        {
            // LOG_ERROR("{0} Null file path given!", __FUNCTION__);
            return;
        }

        cJSON* jsonRootObject = cJSON_CreateObject();

        const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
        ToJson((const void*)&objectReference, typeInfo, jsonRootObject, typeInfo->stringName);

        const char* jsonStructureString = cJSON_Print(jsonRootObject);
        WriteStringToFile(jsonStructureString, absoluteFilePath);
        free((char*)jsonStructureString);

        cJSON_Delete(jsonRootObject);
    }

    void FromJson(const cJSON* objJson, const Mirror::TypeInfo* const objTypeInfo, void* obj);

    template <class T>
    void FromFile(const char* absoluteFilePath, T& objectReference, bool createIfMissing = false)
    {
        if (!absoluteFilePath)
        {
            // LOG_ERROR("{0} Null file path given!", __FUNCTION__);
            return;
        }

        if (const char* jsonFileBuffer = LoadFile(absoluteFilePath))
        {
            if (cJSON* rootJsonObject = cJSON_Parse(jsonFileBuffer))
            {
                const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
                if (!rootJsonObject->child)
                {
                    // LOG_ERROR("{0} root JSON object has no children in JSON file {1}!", __FUNCTION__, absoluteFilePath);
                }
                else if (strcmp(rootJsonObject->child->string, typeInfo->stringName.c_str()) != 0)
                {
                    // LOG_ERROR("{0} root 1st level object name {1} doesn't match given type of {2}!", __FUNCTION__, rootJsonObject->child->string, typeInfo->stringName.c_str());
                }
                else
                {
                    FromJson(rootJsonObject->child, Mirror::InfoForType<T>(), (void*)&objectReference);
                }
                cJSON_Delete(rootJsonObject);
            }
            else
            {
                // LOG_ERROR("{0} Could not parse JSON file {1}! Possible compile error. Check file for typos", __FUNCTION__, absoluteFilePath);
            }
            delete[] jsonFileBuffer;
        }
        else
        {
            if (createIfMissing)
            {
                ToFile(objectReference, absoluteFilePath);
                // LOG_WARN("{0} File {1} missing so created", __FUNCTION__, absoluteFilePath);
            }
            else
            {
                // LOG_ERROR("{0} Error loading JSON file {1}!", __FUNCTION__, absoluteFilePath);
            }
        }
    }

    void SerializeTest();

}

