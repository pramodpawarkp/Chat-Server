#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../include/client_handler.h"
#include "../include/protocol.h"
#include "../include/router.h"

void* handle_client(void* arg)
{
    int client_socket = *(int*)arg;
    free(arg); // important

    char buffer[1024];
    char command[50];
    char data[1024];
    char username[50] = "Anonymous";

    printf("Thread started for client: %d\n", client_socket);

    while (1)
    {
        int bytes = recv(client_socket, buffer, sizeof(buffer)-1, 0);

        if (bytes <= 0)
        {
            printf("Client disconnected: %d\n", client_socket);
            remove_client(client_socket);
            break;
        }

        buffer[bytes] = '\0';

        int cmd = parse_message(buffer, command, data);

        switch (cmd)
        {
            case CMD_LOGIN:
                strcpy(username, data);
                set_username(client_socket, username);
                printf("Client %d logged in as %s\n", client_socket, username);
                send(client_socket, "Login successful\n", 17, 0);
                break;

            case CMD_MSG:
                printf("%s: %s\n", username, data);
                broadcast_message(client_socket, data);
                break;

            default:
                send(client_socket, "Invalid command\n", 16, 0);
                break;
        }
    }

    close(client_socket);
    return NULL;
}