#include "server.hh"
#include "client.hh"
#include "DBManager.hh"
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

bool Login_Server::Setup(int port) {
    epoll_fd = epoll_create(1);       
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in ServerAddr, cli_ServerAddr;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    int bindfail = bind(socket_fd, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
    if (bindfail < 0) return false;
    int listenfail = listen(socket_fd, DEFAULT_QUEUE_SIZE);
    if (listenfail < 0) return false;
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socket_fd;
    DBManager.Setup();
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd,&event);
    return true;
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
                clients.emplace_back(client(client_fd));
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
            } 
            else if (events[i].events == EPOLLIN && events[i].data.fd != socket_fd) {
                memset(m_buf, 0, DEFAULT_PROTOCOL_SIZE);
                int n = recv(events[i].data.fd, m_buf,DEFAULT_PROTOCOL_SIZE, 0);
                if (n == 0) { 
                    CloseHandle(events[i].data.fd); 
                    continue;
                }
                for (auto& client : clients) {
                    if (client.socket_fd != events[i].data.fd) continue;
                    if (!client.LoginSign.empty()) {
                        if (client.LoginSign[0] == (char)LoginSig::Register) { 
                            if (client.nickname.empty()) {
                                client.nickname.append(m_buf);
                            }
                            else {
                                if (client.password.empty()) {
                                    client.password.append(m_buf);
                                    RegisterHandle(events[i].data.fd, client); 
                                }
                            }
                        }
                        else if (client.LoginSign[0] == (char)LoginSig::Msg) { 
                            // TODO : Check This Message is received from Logined User.
                            MessageHandle();
                        }
                        else if (client.LoginSign[0] == (char)LoginSig::Login) {
                            if (client.id.empty()) {
                                // TODO : Check Json File if there is a appropriate ID.
                                client.id.append(m_buf);
                            }
                            else {
                                if (client.password.empty()) {
                                    client.password.append(m_buf);
                                    LoginHandle(events[i].data.fd, client); 
                                }
                            }
                        }
                    }
                    else if (client.LoginSign.empty()) {
                        constexpr int flags = 0;
                        client.LoginSign.clear();
                        client.LoginSign.append(m_buf);
                    } 
                }
            }
        }
    }
}
bool Login_Server::LoginHandle(int socket_fd, client& client) {
    // TODO : Set client.nickname Jsonfile's nickname.
    return true;
}
bool Login_Server::RegisterHandle(int socket_fd, client& client) {
    client.id.append(client.nickname + std::to_string(DBManager.GetMemberLength()));
    std::string buff("Your id is : " + client.id);
    send(socket_fd, buff.c_str(), buff.size(), 0);
    puts("Login Success");
    puts(client.nickname.c_str());
    puts(client.password.c_str());
    DBManager.AddClients(client);
    return true;
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