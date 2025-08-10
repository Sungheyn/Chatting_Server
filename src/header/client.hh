#pragma once
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
struct client {
    client(int socket_fd,const char* id,const char* nickname,const char* password) 
    : socket_fd(socket_fd), id(id), nickname(nickname), password(password) {}
    client(int socket_fd) : socket_fd(socket_fd) {}
    int socket_fd;
    std::string id;
    std::string nickname;
    std::string password;
    std::string LoginSign;
};
struct room {
    std::vector<client> clients; 
    bool SendMessageToAll();
    bool RecvMessage();
    bool CheckMessage();
    bool SaveMessage();
};