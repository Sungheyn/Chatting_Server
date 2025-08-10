#pragma once
#include "client.hh"
#include "rapidjson/document.h"
#include <string>

class ClientDBManager {
    std::string buf = std::string("");
    std::string location;
    rapidjson::Document d;
    void ApplyChange();
public:
    int GetMemberLength();
    ClientDBManager(const char* loc);
    void SetJsonFileLocation(const char* loc);
    ClientDBManager();
    void Setup();
    void AddClients(client& clients);
};