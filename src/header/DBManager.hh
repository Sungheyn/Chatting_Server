#pragma once
#include "client.hh"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/ostreamwrapper.h"
#include <fstream>
#include <string>

class ClientDBManager {
    std::ifstream file;
    std::ofstream ofile;
    std::string buf;
    rapidjson::Document d;
    void ApplyChange();
public:
    int GetMemberLength();
    ClientDBManager();
    void AddClients(client& clients);
};
