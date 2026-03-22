#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../include/router.h"

client_t clients[MAX_CLIENTS];
int client_count = 0;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client(int socket)
{
    pthread_mutex_lock(&clients_mutex);

    if (client_count < MAX_CLIENTS)
    {
        clients[client_count].socket = socket;
        strcpy(clients[client_count].username, "Anonymous");
        client_count++;
    }

    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int socket)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].socket == socket)
        {
            clients[i] = clients[client_count - 1];
            client_count--;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void set_username(int socket, char* username)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].socket == socket)
        {
            strcpy(clients[i].username, username);
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_message(int sender_socket, char* message)
{
    pthread_mutex_lock(&clients_mutex);

    char sender[50] = "Unknown";

    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].socket == sender_socket)
        {
            strcpy(sender, clients[i].username);
            break;
        }
    }

    char buffer[1100];
    snprintf(buffer, sizeof(buffer), "%s: %s\n", sender, message);

    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].socket != sender_socket)
        {
            send(clients[i].socket, buffer, strlen(buffer), 0);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}