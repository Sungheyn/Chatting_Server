#pragma once
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
struct client {
    client(int socket_fd, std::string id, char* nickname, char* password) {
        this->socket_fd = socket_fd;
        this->id = id;
        this->nickname = std::string(nickname);
        this->password = std::string(password);
    }
    int socket_fd;
    std::string id;
    std::string nickname;
    std::string password;
};
struct room {
    std::vector<client> clients; 
    bool SendMessageToAll();
    bool RecvMessage();
    bool CheckMessage();
    bool SaveMessage();
};