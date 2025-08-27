#include "server.hh"
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

int Login_Server::Setup(int port) {
    epoll_fd = epoll_create(1);       
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in ServerAddr, cli_ServerAddr;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    int bindfail = bind(socket_fd, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (bindfail < 0) return -1;
    int listenfail = listen(socket_fd, DEFAULT_QUEUE_SIZE);
    if (listenfail < 0) return -1;
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd,&event);
    return socket_fd;
};
bool Login_Server::ReadyForRecv() {
    epoll_event events[DEFAULT_EVENT_SIZE];
    epoll_event event;
    while (true) {
        int cnt = epoll_wait(epoll_fd, events, DEFAULT_EVENT_SIZE, -1);
        for (int i = 0; i < cnt; i++) {
            if (events[i].events == EPOLLIN && events[i].data.fd == socket_fd) {
                puts("Event Found");
                sockaddr_in client_addr;
                socklen_t s_addr_len = sizeof(client_addr);
                int client_fd = accept(socket_fd, (sockaddr*)&client_addr, &s_addr_len);
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                packethandler.AddClient(client_fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
            } 
            else if (events[i].events == EPOLLIN && events[i].data.fd != socket_fd) {
                int n = packethandler.RecvHandle(events[i].data.fd);
                if (n == 0) { 
                    CloseHandle(events[i].data.fd); 
                    packethandler.CloseHandle(events[i].data.fd);
                    continue;
                }
                packethandler.PacketHandle(events[i]);
                // Handle Packet
            }
        }
    }
}
bool Login_Server::CloseHandle(int socket_fd) {
    close(socket_fd);
    return true;
}
