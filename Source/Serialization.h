
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

    void SerializeJsonObject(const void* obj, const Mirror::ClassInfo* classInfo, cJSON* jsonObj);

    template <class T>
    void SerializeObject(const T& obj, cJSON* jsonObj)
    {
        SerializeJsonObject((void*)&obj, Mirror::InfoForClass<T>(), jsonObj);
    }
}
