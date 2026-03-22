#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../include/client_handler.h"

void* handle_client(void* arg)
{
    int client_socket = *(int*)arg;
    free(arg); // important

    char buffer[1024];

    printf("Thread started for client: %d\n", client_socket);

    while (1)
    {
        int bytes = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes <= 0)
        {
            printf("Client disconnected: %d\n", client_socket);
            break;
        }

        buffer[bytes] = '\0';

        printf("Client %d: %s\n", client_socket, buffer);

        // Echo back
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return NULL;
}