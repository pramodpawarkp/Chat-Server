#ifndef ROUTER_H
#define ROUTER_H

#define MAX_CLIENTS 100

typedef struct {
    int socket;
    char username[50];
} client_t;

void add_client(int socket);
void remove_client(int socket);
void set_username(int socket, char* username);
void broadcast_message(int sender_socket, char* message);
void send_private_message(int sender_socket, char* target, char* message);
void list_users(int client_socket);
#endif