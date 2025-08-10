#include "server.hh"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
void SetLoginChar(LoginProtocol* const dest, const char* nick, int const nickLen, const char* pass) {
    dest->Content[0] = 'L';
    strcpy(dest->Content+1, nick);   
    strcpy(dest->Content+nickLen+1, "|");
    strcpy(dest->Content+nickLen+2, pass);
}
void RecvThread(int socket_fd) {
    while (true) {
        char buf[1024];
        recv(socket_fd,buf,1024,0);
        puts(buf);
    }
}
int main() {
        int sockets = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in serverAddr;
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(8080);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        int co = connect(sockets, (sockaddr*)&serverAddr, sizeof(serverAddr));
        std::string s;
        std::string p;
        std::string n;
        std::cout << "Input nickname : ";
        std::getline(std::cin, n);
        std::cout << "Input password : ";
        std::getline(std::cin, p);
        char buf[DEFAULT_PROTOCOL_SIZE];
        LoginProtocol proto;
        SetLoginChar(&proto, n.c_str(), n.length(), p.c_str());
        send(sockets, proto.Content, DEFAULT_PROTOCOL_SIZE, 0);
        std::thread t1(RecvThread, sockets);
        while (1) {
            std::getline(std::cin, s);
            if (s =="c") { close(sockets); }
            send(sockets, buf, s.size(), 0);
        }
}