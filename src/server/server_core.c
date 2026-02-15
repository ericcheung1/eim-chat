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

int *build_client_socket_list(int max_clients) {
    int *client_socket_list = malloc(max_clients * sizeof(int));
    memset(client_socket_list, 0, sizeof(*client_socket_list));

    return client_socket_list;
}

// TODO: fix build_fd_set
int build_fd_set(fd_set *read_fds, int listen_socket, int max_clients, int client_list[]) {
    FD_ZERO(read_fds);
    FD_SET(listen_socket, read_fds);
    int max_sd = listen_socket;

    for (int i = 0; i < max_clients; i++) {
        int sd = client_list[i];
        if (sd > 0) 
            FD_SET(sd, read_fds);

        if (sd > max_sd) 
            max_sd = sd;
    }

    return max_sd;
}


// TODO: accept_new_client, remove_client, 
void accept_new_client(int listen_socket, fd_set *read_fds, int client_socket[], int max_clients) {
    struct sockaddr_in cli_adr;
    socklen_t cli_adr_len = sizeof(cli_adr);
    int new_socket;
    char welcome_msg[] = {"Welcome to EIM!\n"};

    if (FD_ISSET(listen_socket, read_fds)) {
        if ((new_socket = accept(listen_socket, (SA*)&cli_adr, &cli_adr_len)) < 0) {
            printf("accepting connection failed\n");
            exit(0);
        }

        printf("new socket connection as fd #%d\n", new_socket);
        write(new_socket, welcome_msg, sizeof(welcome_msg));

        for (int i = 0; i < max_clients; i++) {
            if (client_socket[i] == 0) {
                client_socket[i] = new_socket;
                printf("adding client socket to list as #%d\n", i);
                break;
            }
        }
    }
}

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

