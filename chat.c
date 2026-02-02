#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void chat(int connfd) {

    char buffer[1024];

    while (1) {
        // TODO: Change to memset? memset(s, 0, n)
        bzero(buffer, 1024);

        read(connfd, buffer, sizeof(buffer));

        FILE *fptr = fopen("chat_log.txt", "a");
        if (fptr == NULL) {
            printf("Error in opening chat log...\n");
            exit(0);
        }

        fprintf(fptr, "Client: %s\n", buffer);
        printf("From client: %s\n", buffer);
        fclose(fptr);

        if (strncmp("exit", buffer, 4) == 0) {
            printf("Server exit...\n");
            close(connfd);
            break;
        }
    }
}