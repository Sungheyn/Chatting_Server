#pragma once

#include "MesssageDBManager.hh"
#include "client.hh"
#include "DBManager.hh"
#include <sys/epoll.h>
#include <vector>
class PacketHandler {
    int socket_fd;
    char m_buf[1024];
    MDBManager mdbmanager;
    ClientDBManager dbmanager;
    std::vector<client> clients;
    bool RegisterHandle(int socket_fd, client& client);
    bool LoginHandle(int socket_fd, client& client);
    bool MessageHandle(int i);
    bool AcceptHandle();
    public:
    int RecvHandle(int fd);
    void CloseHandle(int socket_fd);
    PacketHandler(const char* loc, const char* mdbloc) : socket_fd(0), mdbmanager(mdbloc) { 
        dbmanager.SetJsonFileLocation(loc);
        dbmanager.Setup();
    };
    void AddClient(int client_fd);
    void PacketHandle(epoll_event events);
};