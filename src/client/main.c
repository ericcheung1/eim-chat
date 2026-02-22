#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "client_core.h"

int main() {

    fd_set readfds;
    char username_buff[1024];

    int request_socket = start_client_socket();
    get_username(username_buff);

    while (1) {
        int max_sd = build_fd_set(&readfds, request_socket);

        int activity = select(max_sd+1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) 
            printf("select() failed...!\n");

        if (accept_broadcast(request_socket, &readfds) == 0) {
            printf("sever was shut down, closing client...\n");
            break;
        }
        if (send_messages(request_socket, &readfds, username_buff) == 0) {
            printf("closing client...\n");
            break;
        }
    }

    close(request_socket);

    return 0;
}