#pragma once
#include "DBManager.hh"
#include "client.hh"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#define DEFAULT_QUEUE_SIZE 10
#define DEFAULT_PROTOCOL_SIZE 1024
#define DEFAULT_EVENT_SIZE 512
enum class LoginSig {
    Login = 'L', Msg = 'M',
    Register = 'R'
};
class Login_Server{
    char m_buf[DEFAULT_PROTOCOL_SIZE];
    std::vector<client> clients;
    int epoll_fd, socket_fd;
    std::vector<room> Rooms;
    ClientDBManager DBManager;
    bool MessageHandle();
    bool CloseHandle(int socket_fd);
    bool RegisterHandle(int socket_fd, client& client);
    bool LoginHandle(int socket_fd, client& client);
public:
    Login_Server(const char* loc) {
        DBManager.SetJsonFileLocation(loc);
        socket_fd = 0;
        epoll_fd = 0;
    }
    bool ReadyForRecv();
    bool Setup(int port);
    bool Stop();

};