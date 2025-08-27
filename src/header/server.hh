#pragma once
#include "PacketHandler.hh"
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
class Login_Server {
    PacketHandler packethandler;
    int epoll_fd, socket_fd;
    std::vector<room> rooms;
    bool CloseHandle(int socket_fd);
public:
    Login_Server(const char* loc, const char* mdbloc) : packethandler(loc, mdbloc) {
        socket_fd = 0;
        epoll_fd = 0;
    }
    bool ReadyForRecv();
    int Setup(int port);
    bool Stop();

};