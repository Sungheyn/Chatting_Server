#include "server.hh"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "main.h"
int main(void) {
    Login_Server LoginServer(FILE_PATH, MDB_FILE_PATH);
    if (!LoginServer.Setup(PORT)) { return -1;}
    LoginServer.ReadyForRecv();
    return 0;
}
