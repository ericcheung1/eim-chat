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

    int listen_socket;

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (listen_socket == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else {
        printf("socket created successfully...!\n");
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(8080);

    if (bind(listen_socket, (SA*)&server_addr, sizeof(server_addr)) != 0) {
        printf("socket binding failed...\n");
        exit(0);
    } else {
        printf("socket binded successfully...!\n");
    }

    if (listen(listen_socket, 5) != 0) { // listen(fd, backlog);
        printf("server listening failed...\n");
        exit(0);
    } else {
        printf("server listening sucessfully...!\n");
    }

    return listen_socket;
}

int *build_client_socket_list(int max_clients) {
    int *client_socket_list = malloc(max_clients * sizeof(int));
    memset(client_socket_list, 0, sizeof(*client_socket_list));

    return client_socket_list;
}

// TODO: fix build_fd_set
int build_fd_set(fd_set *readfds, int listen_socket, int max_clients, int client_list[]) {
    FD_ZERO(readfds);
    FD_SET(STDIN_FILENO, readfds);
    FD_SET(listen_socket, readfds);
    int max_sd = listen_socket;

    for (int i = 0; i < max_clients; i++) {
        int sd = client_list[i];
        if (sd > 0) 
            FD_SET(sd, readfds);

        if (sd > max_sd) 
            max_sd = sd;
    }

    return max_sd;
}


// TODO: accept_new_client, remove_client, 
void accept_new_client(int listen_socket, fd_set *readfds, int client_socket[], int max_clients) {
    struct sockaddr_in cli_adr;
    socklen_t cli_adr_len = sizeof(cli_adr);
    int new_socket;

    if (FD_ISSET(listen_socket, readfds)) {
        if ((new_socket = accept(listen_socket, (SA*)&cli_adr, &cli_adr_len)) < 0) {
            printf("accepting connection failed\n");
            exit(0);
        }

        printf("new socket connection as fd #%d\n", new_socket);

        for (int i = 0; i < max_clients; i++) {
            if (client_socket[i] == 0) {
                client_socket[i] = new_socket;
                printf("adding client socket to list in slot #%d\n", i);
                break;
            }
        }
    }
}

void handle_client_data(int max_clients, int client_socket[], fd_set *readfds) {
    int sd;
    char client_msg_buf[1024] = {0};
    int value_read;

    for (int i = 0; i < max_clients; i++) {
        sd = client_socket[i];

        if (FD_ISSET(sd, readfds)) {
            memset(client_msg_buf, 0, sizeof(client_msg_buf));
            value_read = read(sd, client_msg_buf, sizeof(client_msg_buf));
            client_msg_buf[value_read] = '\0';

            if (value_read == 0) {
                printf("disconnecting client on fd: %d\n", sd);
                close(sd);
                client_socket[i] = 0;
                break;
            }

            for (int j = 0; j < max_clients; j++) {
                int send_sd = client_socket[j];
                if (send_sd != sd && send_sd > 0) 
                    write(send_sd, client_msg_buf, value_read);
            }
            
            FILE *write_ptr = fopen("chat_log.txt", "a");
            if (write_ptr == NULL) {
                printf("error in writing to chat log...\n");
                exit(0);
            }

            fputs(client_msg_buf, write_ptr);
            fclose(write_ptr);
        } 
    }
}

int exit_check(fd_set *readfds) {
    char exit_status_buff[12] = {0};
    int n = 0;

    if (FD_ISSET(STDIN_FILENO, readfds)) {
        while ((exit_status_buff[n++] = getchar()) != '\n');
        exit_status_buff[n] = '\0';

        if (strncmp(exit_status_buff, "exit", 4) == 0) {
            printf("detected exit command...\n");
            return 0;
        }
    }

    return 1;
}