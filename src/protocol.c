#include <string.h>
#include <stdio.h>
#include "../include/protocol.h"

int parse_message(char* input, char* command, char* data)
{
    char* colon = strchr(input, ':');

    if (!colon)
        return CMD_UNKNOWN;

    // Split into command and data
    *colon = '\0';
    strcpy(command, input);
    strcpy(data, colon + 1);

    if (strcmp(command, "LOGIN") == 0)
        return CMD_LOGIN;

    if (strcmp(command, "MSG") == 0)
        return CMD_MSG;

    return CMD_UNKNOWN;
}