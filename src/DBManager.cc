#include "header/DBManager.hh"
#include "header/rapidjson/document.h"
#include "header/rapidjson/rapidjson.h"
#include "header/rapidjson/stringbuffer.h"
#include "header/rapidjson/filereadstream.h"
#include "header/rapidjson/writer.h"
#include <fstream>
#include <cstdio>

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
    std::ofstream ofile("/home/discopse/Documents/clients.json");
    if (ofile.is_open()) {
        ofile << buf.GetString();
        ofile.close();
    }
}
int ClientDBManager::GetMemberLength() {
    return d.MemberCount();
}
ClientDBManager::ClientDBManager() {
    FILE* fp = std::fopen("/home/discopse/Documents/clients.json", "rb");
    char readbuf[65536];
    rapidjson::FileReadStream is(fp, readbuf, sizeof(readbuf));
    d.ParseStream(is);
    std::fclose(fp);
}