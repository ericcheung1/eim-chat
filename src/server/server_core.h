#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <sys/select.h>

int start_socket();

void accept_conn(int socket_fd);

int build_fd_set(fd_set *read_fds, int listen_socket, int max_clients, int client_list[]);

void handle_client_data(int max_clients, int client_socket[], fd_set *readfds);

#endif