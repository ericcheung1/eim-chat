#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server_core.h"
#include "chat.h"


int main() {

    int socketfd = start_socket();

    accept_conn(socketfd);
    
    // char welcome_msg[] = {"Welcome to EIM!\n"};
    
    // if (connfd) {
    //     write(connfd, welcome_msg, sizeof(welcome_msg));
    // }

    // chat(connfd);

    // close(socketfd);

    return 0;
}