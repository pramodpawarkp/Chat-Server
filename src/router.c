#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../include/router.h"

client_t clients[MAX_CLIENTS];
int client_count = 0;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

static void trim_eol(char *s)
{
    s[strcspn(s, "\r\n")] = '\0';
}

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

    trim_eol(username);

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

void send_private_message(int sender_socket, char* target, char* message)
{
    pthread_mutex_lock(&clients_mutex);

    char sender[50] = "Unknown";
    int target_socket = -1;

    for (int i = 0; i < client_count; i++)
    {
        if (clients[i].socket == sender_socket)
            strcpy(sender, clients[i].username);

        printf("index=%d , stored='%s', target='%s'\n", i,clients[i].username, target);
        if (strcmp(clients[i].username, target) == 0)
            target_socket = clients[i].socket;
    }

    if (target_socket != -1)
    {
        char buffer[1100];
        snprintf(buffer, sizeof(buffer), "[PM from %s]: %s\n", sender, message);
        send(target_socket, buffer, strlen(buffer), 0);
    }
    else
    {
        send(sender_socket, "User not found\n", 15, 0);
    }

    pthread_mutex_unlock(&clients_mutex);
}

void list_users(int client_socket)
{
    pthread_mutex_lock(&clients_mutex);

    char buffer[1024] = "Online users:\n";

    for (int i = 0; i < client_count; i++)
    {
        strcat(buffer, clients[i].username);
        strcat(buffer, "\n");
    }

    send(client_socket, buffer, strlen(buffer), 0);

    pthread_mutex_unlock(&clients_mutex);
}