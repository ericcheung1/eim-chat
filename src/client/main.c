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
    char* user_name = get_username(username_buff);

    while (1) {
        int max_sd = build_fd_set(&readfds, request_socket);

        int activity = select(max_sd+1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) 
            printf("select() failed...!\n");

        accept_broadcast(request_socket, &readfds);
        send_messages(request_socket, &readfds, user_name);
    }

    close(request_socket);

    return 0;
}