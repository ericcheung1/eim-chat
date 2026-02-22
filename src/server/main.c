#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>

#include "server_core.h"

int main() {

    fd_set readfds;
    int max_clients = 5;

    int listen_socket = start_socket();
    int* client_list = build_client_socket_list(max_clients);
    
    while (1) {
        int maxsd = build_fd_set(&readfds, listen_socket, max_clients, client_list);
        int activity = select(maxsd+1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) 
            printf("select() failed...!\n");
        
        if (exit_check(&readfds) == 0) {
            for (int i = 0; i < max_clients; i++) {
                int sd = client_list[i];
                close(sd);
            }
            printf("disconnected all clients and closing program...\n");
            break;
        }

        accept_new_client(listen_socket, &readfds, client_list, max_clients);
        handle_client_data(max_clients, client_list, &readfds);

    } 

    free(client_list);
    close(listen_socket);

    return 0;
}