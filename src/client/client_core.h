#ifndef CLIENT_CORE_H
#define CLIENT_CORE_H

#include <sys/select.h>

int start_client_socket();

void get_username(char user_name[]);

int build_fd_set(fd_set* read_fds, int client_socket);

int accept_broadcast(int request_socket, fd_set *read_fds);

int send_messages(int request_socket, fd_set *read_fds, char username[]);

#endif