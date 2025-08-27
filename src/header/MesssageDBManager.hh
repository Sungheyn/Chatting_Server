#pragma once
#include "DBManager.hh"

class MDBManager : DBManager {
    public:
    MDBManager(const char* loc);
    bool AddMessage(const char* id, const char* value);
};