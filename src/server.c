#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../include/server.h"

void start_server(int port)
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int clients[MAX_CLIENTS];
    int client_count = 0;

    // Initialize clients
    for (int i = 0; i < MAX_CLIENTS; i++)
        clients[i] = -1;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Bind setup
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen
    listen(server_fd, 5);

    printf("Server started on port %d...\n", port);

    while (1)
    {
        client_socket = accept(server_fd, NULL, NULL);

        printf("New client connected: %d\n", client_socket);

        // Store client
        if (client_count < MAX_CLIENTS)
        {
            clients[client_count++] = client_socket;
        }
        else
        {
            printf("Max clients reached!\n");
            close(client_socket);
        }

        // Print connected clients
        printf("Clients: ");
        for (int i = 0; i < client_count; i++)
        {
            printf("%d ", clients[i]);
        }
        printf("\n");
    }

    close(server_fd);
}