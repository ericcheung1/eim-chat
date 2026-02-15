#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>

#include "server_core.h"
#include "chat.h"


int main() {

    fd_set readfds;
    int max_clients = 5;

    int socketfd = start_socket();
    int* client_list = build_client_socket_list(max_clients);
    
    while (1) {
        int maxsd = build_fd_set(&readfds, socketfd, max_clients, client_list);
        int activity = select(maxsd+1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) 
            printf("select() failed...!\n");

        accept_new_client(socketfd, &readfds, client_list, max_clients);
        handle_client_data(max_clients, client_list, &readfds);
    }

    free(client_list);
    close(socketfd);

    return 0;
}