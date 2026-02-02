#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server_core.h"
#include "chat.h"


int main() {

    int socketfd = start_socket();

    int connfd = accept_conn(socketfd);

    chat(connfd);

    close(socketfd);

    return 0;
}