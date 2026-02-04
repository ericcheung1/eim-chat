#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "server_core.h"

#define SA struct sockaddr

int start_socket() {

    int socket_fd;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_fd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket created successfully...!\n");
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(8080);

    if (bind(socket_fd, (SA*)&server_addr, sizeof(server_addr)) != 0) {
        printf("Socket binding failed...\n");
        exit(0);
    } else {
        printf("Socket binded successfully...!\n");
    }

    if (listen(socket_fd, 5) != 0) {
        printf("Server listening failed...\n");
        exit(0);
    } else {
        printf("Server listening sucessfully...\n");
    }

    return socket_fd;
}

int accept_conn(int socket_fd) {
    
    int conn_fd;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    conn_fd = accept(socket_fd, (SA*)&client_addr, &len);
    if (conn_fd < 0) {
        printf("Server accept failed...\n");
        exit(0);
    } else {
        printf("Server accept client successfully...!\n");
    }

    return conn_fd;
}