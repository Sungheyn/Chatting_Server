#include "header/server.hh"
#include "header/client.hh"
#include "header/DBManager.hh"
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

bool Login_Server::Setup() {
    epoll_fd = epoll_create(1);       
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in ServerAddr, cli_ServerAddr;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(8080);
    int bindfail = bind(socket_fd, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (bindfail < 0) return false;
    int listenfail = listen(socket_fd, DEFAULT_QUEUE_SIZE);
    if (listenfail < 0) return false;
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd,&event);
    return true;
};
char* Login_Server::ReadNickAndPass(char* buf, int len) {
    for (int i = 1;i < len;i++) {
        if (buf[i] == '|') {
            buf[i] = 0;
            return buf+i;
        }
    }
    return nullptr;
}
bool Login_Server::ReadyForRecv() {
    epoll_event events[512];
    char buf[1024];
    epoll_event event;
    while (true) {
        int cnt = epoll_wait(epoll_fd, events, 511, -1);
        for (int i = 0; i < cnt; i++) {
            if (events[i].events == EPOLLIN && events[i].data.fd == socket_fd) {
                puts("Event Found");
                sockaddr_in client_addr;
                socklen_t s_addr_len = sizeof(client_addr);
                int client_fd = accept(socket_fd, (sockaddr*)&client_addr, &s_addr_len);
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
            } else if (events[i].events == EPOLLIN && events[i].data.fd != socket_fd) {
                memset(buf, 0, 1024);
                int flags = 0;
                int n = recv(events[i].data.fd, buf, 1024, flags);
                if (n == 0) {
                    if (CloseHandle(events[i].data.fd)) return false;
                }
                if (buf[0] == 'L') { LoginHandle(buf, events[i].data.fd); }
                else if (buf[0] == 'M') { MessageHandle(); }
            }
        }
    }
}
bool Login_Server::LoginHandle(char* const buf, int socket_fd) {
    if (buf[0] == 'L') {
        char* I = ReadNickAndPass(buf, 1024);          
        std::string id(buf+1);
        id.insert(0, std::to_string(1000+DBManager.GetMemberLength()));
        if (I == nullptr) { return false; }
        client IClient(socket_fd, id, buf+1,I+1);
        std::string buff("Your id is :");
        buff.append(id);
        send(socket_fd, buff.c_str(), buff.size(), 0);
        puts("Login Success");
        puts(IClient.nickname.c_str());
        puts(IClient.password.c_str());
        DBManager.AddClients(IClient);   
        return true;
    } else { return false; }
}
bool Login_Server::CloseHandle(int socket_fd) {
    close(socket_fd);
    return true;
}
bool Login_Server::MessageHandle() {
    return true;
}
bool Login_Server::Stop() {
    return true;
}