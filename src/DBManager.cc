#include "DBManager.hh"
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <cassert>
#include <fstream>

void ClientDBManager::AddClients(client& clients) {
    rapidjson::Value s(rapidjson::kStringType);
    rapidjson::Value a(rapidjson::kObjectType);
    rapidjson::Value b(rapidjson::kStringType);
    rapidjson::Value id(rapidjson::kStringType);
    id.SetString(clients.id.c_str(), clients.id.size());
    s.SetString(clients.nickname.c_str(), clients.nickname.size());
    b.SetString(clients.password.c_str(), clients.password.size());
    a.AddMember("nickname", s, d.GetAllocator());
    a.AddMember("password", b, d.GetAllocator());
    d.AddMember(id.Move(),a.Move(), d.GetAllocator());
    ApplyChange();
}
void ClientDBManager::ApplyChange() {
    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    d.Accept(writer);
    std::ofstream ofile(location);
    if (ofile.is_open()) {
        ofile << buf.GetString();
        ofile.close();
    }
}
int ClientDBManager::GetMemberLength() {
    return d.MemberCount();
}
ClientDBManager::ClientDBManager() {
    location.clear();
}
ClientDBManager::ClientDBManager(const char* loc) {
    location.clear();
    location.append(loc);
}
void ClientDBManager::SetJsonFileLocation(const char* loc) {
    location.clear();
    location.append(loc);
}
void ClientDBManager::Setup() {
    if (location.empty()) { 
        assert(!"You must call ClientDBManager.SetJsonFileLocation(const char* loc) first."); 
        return;
    }
    else {
        std::ifstream ifs(location.c_str());
        if (ifs.is_open()) {
            rapidjson::IStreamWrapper isw(ifs);
            d.ParseStream(isw);
            ifs.close();
        }
    }
}