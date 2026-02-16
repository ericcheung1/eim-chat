#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define SA struct sockaddr

void send_messages(int client_socket) {
    char buff[1024];
    char c;
    int n;

    while (1) {
        memset(buff, 0, sizeof(buff));
        n = 0;

        while (1) {
            c = getchar();
            if (c == '\n') {
                break;
            } else {
                buff[n] = c;
                n++;
            }
        }

        write(client_socket, buff, sizeof(buff));

        if (strncmp("exit", buff, 4) == 0) {
            printf("client exit\n");
            break;
        }

        memset(buff, 0, sizeof(buff));

        read(client_socket, buff, sizeof(buff));
        printf("User: %s\n", buff);
    }
}

int main() {

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    char welcome_buff[1024];
    

    if (client_socket == -1) {
        printf("client socket creation failed...\n");
        exit(0);
    } else {
        printf("client socket created successfully...!\n");
    }

    struct sockaddr_in client_addr;

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    client_addr.sin_port = htons(8080);

    if (connect(client_socket, (SA*)&client_addr, sizeof(client_addr)) != 0) {
        printf("connection with server failed...\n");
        exit(0);
    } else {
        printf("connected to server successfully...!\n");
        read(client_socket, welcome_buff, sizeof(welcome_buff));
        printf("%s\n", welcome_buff);
    }

    
    send_messages(client_socket);

    close(client_socket);

    return 0;
}