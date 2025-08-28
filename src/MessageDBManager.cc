#include "MesssageDBManager.hh" 
#include <cstring>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

MDBManager::MDBManager(const char* loc) {
    location.clear();
    location.append(loc);
    Setup();
}
bool MDBManager::AddMessage(const char* id, const char* value) {
    auto& allocator = d.GetAllocator();

    // Create the new object to be added: {"value": "..."}
    rapidjson::Value newObject(rapidjson::kObjectType);
    rapidjson::Value newValue(value, allocator);
    newObject.AddMember("value", newValue, allocator);

    // Check if a member with the key 'id' exists and if it is an array.
    if (d.HasMember(id) && d[id].IsArray()) {
        // If it exists and is an array, push the new object into it.
        d[id].PushBack(newObject, allocator);
    } else {
        // If it does not exist, or it exists but is NOT an array,
        // we create a new array and add it (or replace the old value).

        // If the member exists, we must remove it first before adding a new one with the same key.
        if (d.HasMember(id)) {
            d.RemoveMember(id);
        }

        // Create a new array.
        rapidjson::Value newArray(rapidjson::kArrayType);
        newArray.PushBack(newObject, allocator);

        // Create the key for the new member.
        rapidjson::Value idValue(id, allocator);

        // Add the new array member to the document.
        d.AddMember(idValue, newArray, allocator);
    }

    ApplyChange();
    return true;
}