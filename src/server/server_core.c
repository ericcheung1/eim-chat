#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>

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

void accept_conn(int socket_fd) {
    
    // int conn_fd;
    int client_socket[5];
    int max_clients = 5;
    // int max_sd;
    // int sd;
    int activity;
    int new_socket;
    // int valread;
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    fd_set readfds;
    // char buffer[1024];
    char greeting[] = {"Hello!\n"};

    // memset(client_socket, 0, sizeof(client_socket));
    for (int i = 0; i < max_clients; i++) {
        client_socket[i] = 0;
    }

    // select loop
    while (1) {
        int max_sd = build_fd_set(&readfds, socket_fd, max_clients, client_socket);

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        printf("activity right now: %d\n", activity);

        if ((activity < 0) && (errno != EINTR)) {
            printf("Select failed...\n");
        }

        // accepts new client sockets
        if (FD_ISSET(socket_fd, &readfds)) {
            if ((new_socket = accept(socket_fd, (SA*)&client_addr, &len)) < 0) {
                printf("accepting failed\n");
                exit(0);
            }

            printf("new socket: %d\n", new_socket);
            send(new_socket, greeting, strlen(greeting),0);

            for (int i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    printf("Adding to list as %d\n", i);
                    break;
                }
            }
        }
        
        handle_client_data(max_clients, client_socket, &readfds);
    }
}

// TODO: fix build_fd_set
int build_fd_set(fd_set *read_fds, int listen_socket, int max_clients, int client_list[]) {
    FD_ZERO(read_fds);
    FD_SET(listen_socket, read_fds);
    int max_sd = listen_socket;

    for (int i = 0; i < max_clients; i++) {
        int sd = client_list[i];

        if (sd > 0) {
            FD_SET(sd, read_fds);
        }

        if (sd > max_sd) {
            max_sd = sd;
        }
    }

    return max_sd;
}


// TODO: accept_new_client, remove_client, 
// int accept_new_client(int listen_socket, fd_set *read_fds)

// void remove_client(int client_fds[], int index);

void handle_client_data(int max_clients, int client_socket[], fd_set *readfds) {
    int sd;
    char client_msg_buf[1024];
    int value_read;

    for (int i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (FD_ISSET(sd, readfds)) {
                memset(client_msg_buf, 0, sizeof(client_msg_buf));
                value_read = read(sd, client_msg_buf, sizeof(client_msg_buf));
                FILE *fptr = fopen("chat_log.txt", "a");
                if (fptr == NULL) {
                    printf("Error in opening chat log...\n");
                    exit(0);
                }

                fprintf(fptr, "Client: %s\n", client_msg_buf);
                fclose(fptr);

                if (strncmp("exit", client_msg_buf, 4) == 0) {
                    printf("Disconnecting client on fd: %d\n", sd);
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    // set the string terminating NULL byte
                    // on the end of the data read
                    client_msg_buf[value_read] = '\0';
                    send(sd, client_msg_buf, strlen(client_msg_buf), 0);
                }
            } 
        }
}

