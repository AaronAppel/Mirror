
#include "../Libraries/cJSON/cJSON.h"

#include "Mirror.h"

namespace Serialization
{
    void DeserializeJsonObject(const cJSON* jsonObj, const Mirror::ClassInfo* classInfo, void* obj);

    template <class T>
    void DeserializeJsonObject(const cJSON* jsonObj, T& obj)
    {
        DeserializeJsonObject(jsonObj, Mirror::InfoForClass<T>(), (void*)&obj);
    }

    void DeserializeJsonPrimitive(const cJSON* jsonObj, const Mirror::ClassInfo* classInfo, void* obj);

    template <class T>
    void DeserializeJsonPrimitive(const T& obj, cJSON* jsonObj)
    {
        SerializeJsonObject((void*)&obj, Mirror::InfoForClass<T>(), jsonObj);
    }

    void SerializeJsonObject(const void* obj, const Mirror::ClassInfo* classInfo, cJSON* jsonObj);

    template <class T>
    void SerializeObject(const T& obj, cJSON* jsonRoot)
    {
        cJSON* arr = cJSON_CreateArray(); // #TODO Play with formatting for cJSON* cObj = cJSON_CreateObject();
        arr->string = _strdup(Mirror::InfoForType<T>()->stringName.c_str());
        cJSON_AddItemToArray(jsonRoot, arr);

        SerializeJsonObject((void*)&obj, Mirror::InfoForClass<T>(), arr);
    }

    void SerializeJsonType(const void* obj, const Mirror::Field& field, cJSON* jsonObj);
    void SerializeJsonType(const void* obj, const Mirror::TypeInfo& typeInfo, cJSON* jsonObj);

    template <class T>
    void SerializeType(const T& obj, cJSON* jsonObj)
    {
        const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();

        cJSON* cObj = cJSON_CreateObject();
        cJSON_AddItemToObject(jsonObj, typeInfo->stringName.c_str(), cObj);

        SerializeJsonType((void*)&obj, (Mirror::TypeInfo&)typeInfo, jsonObj);
    }

}
