#ifndef SERVER_CORE_H
#define SERVER_CORE_H

int start_socket();

int *build_client_socket_list(int max_clients);

int build_fd_set(fd_set *read_fds, int listen_socket, int max_clients, int client_list[]);

void accept_new_client(int listen_socket, fd_set *read_fds, int client_socket[], int max_clients);

void handle_client_data(int max_clients, int client_socket[], fd_set *readfds);

#endif