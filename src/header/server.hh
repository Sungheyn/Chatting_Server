#pragma once
#include "DBManager.hh"
#include "client.hh"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#define DEFAULT_QUEUE_SIZE 10
#define DEFAULT_PROTOCOL_SIZE 1024
struct LoginProtocol {
    char Content[DEFAULT_PROTOCOL_SIZE+1];
};
class Login_Server{
    char m_buf[1024];
    std::vector<client> clients;
    int epoll_fd, socket_fd;
    std::vector<room> Rooms;
    ClientDBManager DBManager = ClientDBManager();
    char* ReadNickAndPass();
    bool MessageHandle();
    bool CloseHandle(int socket_fd);
    bool LoginHandle(int socket_fd);
public:
    Login_Server(const char* loc) {
        DBManager.SetJsonFileLocation(loc);
        socket_fd = 0;
        epoll_fd = 0;
    }
    bool ReadyForRecv();
    bool Setup();
    bool Stop();

};