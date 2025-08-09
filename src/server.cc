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

	/** Hard-coded port values if other process is using port 8080, the programme dies */
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
char* Login_Server::ReadNickAndPass(int len) {
	if(len > sizeof(m_buf)) {
		assert(!"len is greated than expected");
		return nullptr;
	}

	for (int i = 1;i < len && m_buf[i] /* null_terminator */;i++) {
		if (m_buf[i] == '|') /** dim for id/passwd. Seems fishy. */ {
			m_buf[i] = 0;
			return m_buf+i;
		}
	}

	return nullptr;
}
bool Login_Server::ReadyForRecv() {
	epoll_event events[512];
	epoll_event event;
	while (true) {
		int cnt = epoll_wait(epoll_fd, events, (sizeof(events) / sizeof(events[0])), -1);
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
				memset(this->m_buf, 0, sizeof(m_buf));
				constexpr int flags = 0; /**  */
				int n = recv(events[i].data.fd, this->m_buf, sizeof(m_buf), flags);
				if (n == 0) {
					if (CloseHandle(events[i].data.fd)) return false;
				}
				if (m_buf[0] == 'L') { LoginHandle(events[i].data.fd); }
				else if (m_buf[0] == 'M') { MessageHandle(); }
			}
		}
	}
}
bool Login_Server::LoginHandle(int socket_fd) {
	if (m_buf[0] == 'L') {
		char* I = ReadNickAndPass(1024);
		std::string id(m_buf+1);
		id.insert(0, std::to_string(1000+DBManager.GetMemberLength()));
		if (I == nullptr) { return false; }
		client IClient(socket_fd, id.c_str(), m_buf+1,I+1);
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
