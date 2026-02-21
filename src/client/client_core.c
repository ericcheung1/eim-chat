#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include "client_core.h"

#define SA struct sockaddr

int start_client_socket() {
    int request_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (request_socket == -1) {
        printf("client socket creation failed...\n");
        exit(0);
    } else {
        printf("client socket created successfully...!\n");
    }

    struct sockaddr_in client_addr;

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    client_addr.sin_port = htons(8080);

    if (connect(request_socket, (SA*)&client_addr, sizeof(client_addr)) != 0) {
        printf("connection with server failed...\n");
        exit(0);
    } else {
        printf("connected to server successfully...!\n");
        printf("welcome to eim...!\n");
    }

    return request_socket;
}

char *get_username(char user_name[]) {

    printf("enter username: ");
    fgets(user_name, strlen(user_name), stdin);

    user_name[strlen(user_name)-1] = ':';
    user_name[strlen(user_name)] = ' ';

    return user_name;
}

int build_fd_set(fd_set* read_fds, int request_socket) {
    int max_sd = request_socket;
    FD_ZERO(read_fds);
    FD_SET(request_socket, read_fds);
    FD_SET(STDIN_FILENO, read_fds);

    return max_sd;
}

void accept_broadcast(int request_socket, fd_set *read_fds) {
    char incoming_buff[1024];

    if (FD_ISSET(request_socket, read_fds)) 
        read(request_socket, incoming_buff, sizeof(incoming_buff));

    printf("%s\n", incoming_buff);
}

void send_messages(int request_socket, fd_set *read_fds, char username[]) {
    char outgoing_buff[1024];
    int username_len = strlen(username);
    strcpy(outgoing_buff, username);
    int n = username_len;

    if (FD_ISSET(STDIN_FILENO, read_fds)) {
        while ((outgoing_buff[n++] = getchar()) != '\n');
        write(request_socket, outgoing_buff, strlen(outgoing_buff));
    }
}