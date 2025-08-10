#include <cstdio>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
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
        int a;
        std::cout << "For login : 0, For Register : 1. : ";
        std::cin >> a; 
        if (a == 1) {
            send(sockets, "L", 1, 0);
            std::cout << "Input id : ";
            std::getline(std::cin, n);
            send(sockets, n.c_str(), n.size(), 0);
            std::cout << "Input password : ";
            std::getline(std::cin, p);
            send(sockets, p.c_str(), p.size(), 0);
        } else if (a == 0) {
            send(sockets, "R", 1, 0);
            std::cout << "Input nickname : ";
            std::getline(std::cin, n);
            send(sockets, n.c_str(), n.size(), 0);
            std::cout << "Input password : ";
            std::getline(std::cin, p);
            send(sockets, p.c_str(), p.size(), 0);
        }
        
        char buf[1024];
        std::thread t1(RecvThread, sockets);
        while (1) {
            std::getline(std::cin, s);
            if (s =="c") { close(sockets); }
            send(sockets, buf, s.size(), 0);
        }
}