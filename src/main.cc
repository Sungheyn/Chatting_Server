#include "header/server.hh"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
int main(void) {
    Login_Server LoginServer;
    if (!LoginServer.Setup()) { return -1;}
    LoginServer.ReadyForRecv();
    return 0;
}
