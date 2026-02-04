#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void chat(int connfd) {

    char client_msg_buf[1024];
    char client_name_buf[] = {"Client: "};

    while (1) {
        // TODO: Change to memset? memset(s, 0, n)
        bzero(client_msg_buf, 1024);

        read(connfd, client_msg_buf, sizeof(client_msg_buf));

        FILE *fptr = fopen("chat_log.txt", "a");
        if (fptr == NULL) {
            printf("Error in opening chat log...\n");
            exit(0);
        }

        fprintf(fptr, "Client: %s\n", client_msg_buf);
        printf("From Client: %s\n", client_msg_buf);
        write(connfd, client_name_buf, sizeof(client_name_buf));
        write(connfd, client_msg_buf, sizeof(client_msg_buf));
        fclose(fptr);

        if (strncmp("exit", client_msg_buf, 4) == 0) {
            printf("Server exit...\n");
            close(connfd);
            break;
        }
    }
}