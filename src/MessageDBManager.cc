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
    rapidjson::Value obj(rapidjson::kObjectType);
    rapidjson::Value idv(rapidjson::kStringType);
    rapidjson::Value values(rapidjson::kStringType);
    idv.SetString(id, strlen(id));
    values.SetString(value, strlen(value));    
    obj.AddMember("value", values, d.GetAllocator());
    d.AddMember(idv.Move(), obj.Move(), d.GetAllocator());
    ApplyChange();
    return true;
}