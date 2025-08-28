#include "PacketHandler.hh"
#include "server.hh"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

bool PacketHandler::AcceptHandle() {
    puts("Event Found");
    sockaddr_in client_addr;
    socklen_t s_addr_len = sizeof(client_addr);
    int client_fd = accept(socket_fd, (sockaddr*)&client_addr, &s_addr_len);
    clients.emplace_back(client_fd);
    return true;
}
void PacketHandler::AddClient(int client_fd) {
    clients.emplace_back(client(client_fd));
}
int PacketHandler::RecvHandle(int fd) {
    memset(m_buf, 0, DEFAULT_PROTOCOL_SIZE);
    int n = recv(fd, m_buf, DEFAULT_PROTOCOL_SIZE, 0);
    return n;
}
bool PacketHandler::MessageHandle(int i) {
    std::string s(m_buf);
    std::string m;
    m.append(clients[i].id+" : ");
    m.append(s);
    if (!mdbmanager.AddMessage(clients[i].id.c_str(), m_buf)) return false;
    for (int j = 0; j < clients.size();  j++) {
        if (i == j) continue;
        send(clients[j].socket_fd, m.c_str(), m.size(), 0);
    }
    puts(s.c_str());
    return true;
}
void PacketHandler::PacketHandle(epoll_event events) {
    for (int i = 0; i < clients.size(); i++) {
        client& client = clients[i];
        if (client.socket_fd != events.data.fd) continue;
        if (!client.LoginSign.empty()) {
            switch (client.LoginSign[0]) {
                case (char)LoginSig::Register : {
                    if (client.nickname.empty()) {
                        client.nickname.append(m_buf);
                    }
                    else {
                        if (client.password.empty()) {
                            client.password.append(m_buf);
                            RegisterHandle(events.data.fd, client); 
                        }
                    }
                }
                break;
                case (char)LoginSig::Msg : {
                    if (client.SorN) {
                        if (!MessageHandle(i)) {
                            continue;
                        }
                    }
                }
                break;
                case (char)LoginSig::Login : {
                    if (client.id.empty()) {
                        // TODO : Check Json File if there is a appropriate ID.
                        client.id.append(m_buf);
                    }
                    else {
                        if (client.password.empty()) {
                            client.password.append(m_buf);
                            LoginHandle(events.data.fd, client); 
                        }
                    }
                }
                break;
                default : {
                    break;
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
void PacketHandler::CloseHandle(int socket_fd) {
    clients.erase(std::remove_if(clients.begin(), clients.end(), [socket_fd](client& client)->bool { 
        return client.socket_fd == socket_fd;
    }), clients.end());   
}
bool PacketHandler::LoginHandle(int socket_fd, client& client) {
    // TODO : Sync Message Log.
    if(dbmanager.FindMember(client.id.c_str(), client.password.c_str())) {
        puts("Login Success");
        send(socket_fd, "LS", 2, 0);
        client.LoginSign.clear();
        client.LoginSign = "M";
        client.SorN = 1;
        return true;
    }
    return false;
}
bool PacketHandler::RegisterHandle(int socket_fd, client& client) {
    client.id.append(client.nickname + std::to_string(dbmanager.GetMemberLength()));
    std::string buff("Your id is : " + client.id);
    send(socket_fd, buff.c_str(), buff.size(), 0);
    puts("Register Success");
    puts(client.nickname.c_str());
    puts(client.password.c_str());
    dbmanager.AddClients(client);
    return true;
}