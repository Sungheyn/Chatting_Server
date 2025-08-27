#pragma once
#include "client.hh"
#include <rapidjson/document.h>
#include <string>

class DBManager {
    protected:
    std::string location;
    rapidjson::Document d;
    void ApplyChange();
    public:
    void SetJsonFileLocation(const char* loc);
    void Setup();
};

class ClientDBManager final : public DBManager {
    std::string buf;
public:
    bool FindMember(const char* id, const char* pwd);
    int GetMemberLength();
    ClientDBManager(const char* loc);
    ClientDBManager();
    void AddClients(client& clients);
};