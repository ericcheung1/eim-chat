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

void get_username(char user_name[]) {

    printf("enter username: ");
    int n = 0;
    while ((user_name[n++] = getchar()) != '\n');
    printf("\n");

    user_name[n-1] = ':';
    user_name[n] = ' ';
    user_name[n+1] = '\0';
}

int build_fd_set(fd_set* read_fds, int request_socket) {
    int max_sd = request_socket;
    FD_ZERO(read_fds);
    FD_SET(request_socket, read_fds);
    FD_SET(STDIN_FILENO, read_fds);

    return max_sd;
}

int accept_broadcast(int request_socket, fd_set *read_fds) {
    char incoming_buff[1024] = {0};
    int values_read;

    if (FD_ISSET(request_socket, read_fds)) {
        values_read = read(request_socket, incoming_buff, sizeof(incoming_buff));
        if (values_read == 0)
            return 0;
    }

    printf("%s\n", incoming_buff);
    return 1;
}

int send_messages(int request_socket, fd_set *read_fds, char username[]) {
    char outgoing_buff[1024] = {0};
    char input_buff[1024] = {0};
    strcpy(outgoing_buff, username);
    int n = 0;

    if (FD_ISSET(STDIN_FILENO, read_fds)) {
        while ((input_buff[n++] = getchar()) != '\n');
        input_buff[n] = '\0';
        
        if (strncmp(input_buff, "exit", 4) == 0) 
            return 0;

        strcat(outgoing_buff, input_buff);

        write(request_socket, outgoing_buff, strlen(outgoing_buff));
    }
    return 1;
}